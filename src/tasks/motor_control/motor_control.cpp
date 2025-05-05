#include "motor_control.h"
#include "../../globals/globals.h"
#include "../../motor/motor.h"
#include <stdio.h>

void motorControlTask(void* pvParameters)
{
    MotorCommand command;

    while (1) {
        if (xQueueReceive(motorCommandQueue, &command, portMAX_DELAY) == pdTRUE) {
            switch (command.type) {
            case MOTOR_UPDATE_POSITION:
                motorUpdatePosition(command.value);
                break;
            case MOTOR_SET_SETPOINT:
                motorSetSetpoint(command.value);
                if (xSemaphoreTake(g_statusMutex, portMAX_DELAY) == pdTRUE) {
                    g_systemStatus.targetPosition = command.value;
                    xSemaphoreGive(g_statusMutex);
                }
                break;
            default:
                break;
            }
        }
    }
}