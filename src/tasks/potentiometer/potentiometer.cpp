#include "potentiometer.h"
#include <stdlib.h>

void potentiometerTask(void* pvParameters)
{
    int lastPosition = 0;
    pinMode(POT_PIN, INPUT);

    while (1) {
        int rawValue = analogRead(POT_PIN);
        int newPosition = map(rawValue, 0, 1023, 0, 100);

        if (newPosition != lastPosition) {
            MotorCommand cmd;
            cmd.type = MOTOR_UPDATE_POSITION;
            cmd.value = newPosition;
            xQueueSend(motorCommandQueue, &cmd, portMAX_DELAY);

            if (xSemaphoreTake(g_statusMutex, portMAX_DELAY) == pdTRUE) {
                g_systemStatus.currentPosition = newPosition;
                xSemaphoreGive(g_statusMutex);
            }

            lastPosition = newPosition;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
