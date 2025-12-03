#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static const char *TAG = "DEMO_NO_TS";
SemaphoreHandle_t wakeSem;

void vApplicationIdleHook(void)
{
    ESP_EARLY_LOGI(TAG, "IDLE (PRIO=0)");
}

void task1(void *pvParameters)
{
    for (;;) {
        xSemaphoreTake(wakeSem, portMAX_DELAY);

        ESP_LOGI(TAG, "--- Task1 (PRIO=5, event) START");
        TickType_t start_time = xTaskGetTickCount();

        while (xTaskGetTickCount() - start_time < pdMS_TO_TICKS(2000)) {
            // busy loop
        }

        ESP_LOGI(TAG, "--- Task1 END → back to Blocked");
    }
}

void task2(void *pvParameters)
{
    while (1) {
        ESP_LOGI(TAG, "Task2 (PRIO=0, continuous)");
        // Task2 sẽ chiếm CPU hoàn toàn khi Task1 không chạy
    }
}

void app_main(void)
{
    wakeSem = xSemaphoreCreateBinary();

    xTaskCreate(task1, "T1", 4096, NULL, 5, NULL);
    xTaskCreate(task2, "T2", 4096, NULL, 0, NULL);

    ESP_LOGI(TAG, "--- Demo NO TIMESLICING Start ---");

    vTaskDelay(pdMS_TO_TICKS(3000));
    xSemaphoreGive(wakeSem);
}
