#include "tasks.h"
#include "../config/config.h"
#include "button_handler/button_handler.h"
#include "motor_control/motor_control.h"
#include "potentiometer/potentiometer.h"
#include "status_reporter/status_reporter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void createTasks()
{
    xTaskCreate(
        potentiometerTask,
        "PotentiometerTask",
        2048,  // ESP32 typically needs larger stack sizes
        NULL,
        POTENTIOMETER_TASK_PRIORITY,
        NULL);

    xTaskCreate(
        motorControlTask,
        "MotorControl",
        2048,
        NULL,
        MOTOR_TASK_PRIORITY,
        NULL);

    xTaskCreate(
        buttonHandlerTask,
        "ButtonHandler",
        2048,
        NULL,
        BUTTON_TASK_PRIORITY,
        NULL);

    xTaskCreate(
        statusReporterTask,
        "StatusReporter",
        2048,
        NULL,
        STATUS_REPORTER_TASK_PRIORITY,
        NULL);
}