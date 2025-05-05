#include "status_reporter.h"
#include "../../config/config.h"
#include "../../globals/globals.h"
#include <stdio.h>
#include "../../led/led.h"

const TickType_t REPORTING_INTERVAL = pdMS_TO_TICKS(500);

void statusReporterTask(void* pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = REPORTING_INTERVAL;

    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        int currentPos = 0;
        int targetPos = DEFAULT_SETPOINT;
        int lowerBound = 0;
        int upperBound = 100;

        if (xSemaphoreTake(g_statusMutex, portMAX_DELAY) == pdTRUE) {
            currentPos = g_systemStatus.currentPosition;
            targetPos = g_systemStatus.targetPosition;
            lowerBound = g_systemStatus.targetPosition - DEFAULT_HYSTERESIS;
            upperBound = g_systemStatus.targetPosition + DEFAULT_HYSTERESIS;
            xSemaphoreGive(g_statusMutex);
        }

        bool isInBounds = currentPos >= lowerBound && currentPos <= upperBound;
        setLED(isInBounds);

        printf("Status - Pos: %d, Target: %d, Bounds: [%d, %d]\n", currentPos, targetPos, lowerBound, upperBound);
    }
}