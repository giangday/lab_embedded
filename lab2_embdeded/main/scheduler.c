#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_PIN  0  // GPIO nút nhấn (nút BOOT trên nhiều board ESP32)

void cyclic_task(void *pvParameter) {
    while (1) {
        printf("Student ID: 2210830\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // in mỗi 1 giây
    }
}

void acyclic_task(void *pvParameter) {
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pulldown_en(BUTTON_PIN);

    while (1) {
        if (gpio_get_level(BUTTON_PIN) == 0) {
            printf("ESP32\n");
            vTaskDelay(200 / portTICK_PERIOD_MS); // chống dội nút
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    xTaskCreate(cyclic_task, "cyclic_task", 2048, NULL, 5, NULL);
    xTaskCreate(acyclic_task, "acyclic_task", 2048, NULL, 5, NULL);
}
