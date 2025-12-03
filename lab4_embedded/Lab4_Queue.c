#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef enum {
    REQ_FAN = 1,
    REQ_LED = 2,
    REQ_SENSOR = 3
} RequestType;

typedef struct {
    RequestType type;
    int value;
} Request_t;

QueueHandle_t requestQueue;

//  Reception Task

void reception_task(void *param)
{
    Request_t req;

    while (1)
    {
        req.type = REQ_FAN;
        req.value = 5;
        printf("\n[Reception] Send FAN request\n");
        xQueueSend(requestQueue, &req, portMAX_DELAY);

        req.type = REQ_LED;
        req.value = 1;
        printf("[Reception] Send LED request\n");
        xQueueSend(requestQueue, &req, portMAX_DELAY);

        req.type = REQ_SENSOR;
        req.value = 99;
        printf("[Reception] Send SENSOR request\n");
        xQueueSend(requestQueue, &req, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

// FAN TASK
void fan_task(void *param)
{
    Request_t req;

    while (1)
    {
        if (xQueuePeek(requestQueue, &req, portMAX_DELAY))
        {
            if (req.type == REQ_FAN) {
                xQueueReceive(requestQueue, &req, 0);
                printf("[FAN] Processing FAN request, speed=%d\n", req.value);
                vTaskDelay(200 / portTICK_PERIOD_MS);
            } else {
                vTaskDelay(10);
            }
        }
    }
}

// LED TASK
void led_task(void *param)
{
    Request_t req;

    while (1)
    {
        if (xQueuePeek(requestQueue, &req, portMAX_DELAY))
        {
            if (req.type == REQ_LED) {
                xQueueReceive(requestQueue, &req, 0);
                printf("[LED] Processing LED request, state=%d\n", req.value);
                vTaskDelay(200 / portTICK_PERIOD_MS);
            } else {
                vTaskDelay(10);
            }
        }
    }
}

// SENSOR TASK
void sensor_task(void *param)
{
    Request_t req;

    while (1)
    {
        if (xQueuePeek(requestQueue, &req, portMAX_DELAY))
        {
            if (req.type == REQ_SENSOR) {
                xQueueReceive(requestQueue, &req, 0);
                printf("[SENSOR] Processing SENSOR request, val=%d\n", req.value);
                vTaskDelay(200 / portTICK_PERIOD_MS);
            } else {
                vTaskDelay(10);
            }
        }
    }
}

// ERROR HANDLER TASK
void error_task(void *param)
{
    Request_t req;

    while (1)
    {
        if (xQueueReceive(requestQueue, &req, portMAX_DELAY))
        {
            printf("ERROR: No task handled request type=%d\n", req.type);
        }
    }
}

void app_main(void)
{
    printf("=== Lab 04: FreeRTOS Queue Management ===\n");

    requestQueue = xQueueCreate(10, sizeof(Request_t));

    xTaskCreate(reception_task, "Reception", 4096, NULL, 3, NULL);

    xTaskCreate(fan_task,    "Fan",    4096, NULL, 2, NULL);
    xTaskCreate(led_task,    "LED",    4096, NULL, 2, NULL);
    xTaskCreate(sensor_task, "Sensor", 4096, NULL, 2, NULL);

    xTaskCreate(error_task,  "Error",  4096, NULL, 1, NULL);
}
