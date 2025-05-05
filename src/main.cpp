#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "config/config.h"
#include "globals/globals.h"
#include "tasks/tasks.h"
#include "button/button.h"
#include "led/led.h"
#include "motor/motor.h"
#include "serial_comm/serial_comm.h"

void setup()
{
    initSerial();
    initLED();
    initButtons();
    initMotor();
    initGlobals();
    createTasks();

    vTaskStartScheduler();
}

void loop() { }