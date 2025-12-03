#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static const char *TAG = "DEMO";
SemaphoreHandle_t wakeSem;

void vApplicationIdleHook(void)
{
    vTaskDelay(pdMS_TO_TICKS(500));
    ESP_LOGI(TAG, "IDLE  (PRIO = 0, continuous)");
}

void task1(void *pvParameters)
{
    for (;;) {
        xSemaphoreTake(wakeSem, portMAX_DELAY);

        ESP_LOGI(TAG, "---Task1 (PRIO = 5, event)");
        TickType_t start_time = xTaskGetTickCount();
        while ((xTaskGetTickCount() - start_time) < pdMS_TO_TICKS(2000))
        {
        }

        ESP_LOGI(TAG, "Task1 finished → back to Blocked");
    }
}

void task2(void *pvParameters)
{
    for (;;) {
        ESP_LOGI(TAG, "Task2 (PRIO = 0, continuous)");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    wakeSem = xSemaphoreCreateBinary();

    xTaskCreate(task1, "T1", 4096, NULL, 5, NULL);
    xTaskCreate(task2, "T2", 4096, NULL, 0, NULL);

    ESP_LOGI(TAG, "--- Demo Start ---");

    vTaskDelay(pdMS_TO_TICKS(3000));
    xSemaphoreGive(wakeSem);

}
