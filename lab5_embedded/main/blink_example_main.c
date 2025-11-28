/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "freertos/timers.h"

#define TAG "TIMER_LAB"

/* Chu kỳ timer (đơn vị: ticks) */
#define TIMER1_PERIOD_MS 2000
#define TIMER2_PERIOD_MS 3000

/* Giới hạn số lần in */
#define TIMER1_MAX_COUNT 10
#define TIMER2_MAX_COUNT 5
#define BLINK_GPIO 2

/* Biến toàn cục */
static TimerHandle_t timer1 = NULL;
static TimerHandle_t timer2 = NULL;
static int count1 = 0;
static int count2 = 0;

static uint8_t s_led_state = 0;

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
    s_led_state = !s_led_state;
}

static void configure_led(void)
{
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}


/* Callback chung cho cả 2 timer */
static void vTimerCallback(TimerHandle_t xTimer)
{
    int timer_id = (int) pvTimerGetTimerID(xTimer);

    if (timer_id == 1) {
        count1++;
        ESP_LOGI(TAG, "ahihi (%d)", count1);

        // if (count1 >= TIMER1_MAX_COUNT) {
        //     xTimerStop(xTimer, 0);
        //     ESP_LOGW(TAG, "Timer 1 stopped after %d prints", TIMER1_MAX_COUNT);
        // }
    } 
    else if (timer_id == 2) {
        count2++;
        ESP_LOGI(TAG, "ihaha (%d)", count2);

        // if (count2 >= TIMER2_MAX_COUNT) {
        //     xTimerStop(xTimer, 0);
        //     ESP_LOGW(TAG, "Timer 2 stopped after %d prints", TIMER2_MAX_COUNT);
        // }
    }
}


/* Hàm khởi tạo timer */
static void init_timers(void)
{
    /* Tạo Timer 1 - auto-reload */
    timer1 = xTimerCreate(
        "Timer1",
        pdMS_TO_TICKS(TIMER1_PERIOD_MS),
        pdTRUE,      // auto-reload
        (void *)1,   // ID = 1
        vTimerCallback
    );

    /* Tạo Timer 2 - auto-reload */
    timer2 = xTimerCreate(
        "Timer2",
        pdMS_TO_TICKS(TIMER2_PERIOD_MS),
        pdTRUE,      // auto-reload
        (void *)2,   // ID = 2
        vTimerCallback
    );

    if (timer1 == NULL || timer2 == NULL) {
        ESP_LOGE(TAG, "Failed to create timers!");
        return;
    }

    /* Bắt đầu 2 timer */
    xTimerStart(timer1, 0);
    xTimerStart(timer2, 0);
    ESP_LOGI(TAG, "Timers started!");
}


void app_main(void)
{
    ESP_LOGI(TAG, "=== FreeRTOS Software Timer Demo ===");
    init_timers();
    configure_led();

    /* Không cần task chính làm gì thêm */
    while (1) {
        blink_led();
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        // vTaskDelay(pdMS_TO_TICKS(1000));
        
    }
}




