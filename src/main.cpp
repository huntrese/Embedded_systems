/**
 * @file main.cpp
 * @brief Main application entry point with FreeRTOS tasks
 */

 #include <Arduino.h>
 #include <freertos/FreeRTOS.h>
 #include <freertos/task.h>
 
 #include "l298_driver.h"
 #include "motor_control.h"
 #include "lcd_driver.h"
 #include "command_interface.h"
 #include "status_reporter.h"
 
 // Task handles
 TaskHandle_t commandTaskHandle = NULL;
 TaskHandle_t statusTaskHandle = NULL;
 
 // Task stack sizes
 #define COMMAND_TASK_STACK_SIZE 2048
 #define STATUS_TASK_STACK_SIZE 2048
 
 // Task priorities
 #define COMMAND_TASK_PRIORITY 2
 #define STATUS_TASK_PRIORITY 1
 
 void setup() {
     // Initialize all modules
     motor_init();
     status_reporter_init();
     command_interface_init();
     
     // Create FreeRTOS tasks
     xTaskCreate(
         command_task,
         "CommandTask",
         COMMAND_TASK_STACK_SIZE,
         NULL,
         COMMAND_TASK_PRIORITY,
         &commandTaskHandle
     );
     
     xTaskCreate(
         status_reporter_task,
         "StatusTask",
         STATUS_TASK_STACK_SIZE,
         NULL,
         STATUS_TASK_PRIORITY,
         &statusTaskHandle
     );
 }
 
 void loop() {
     // On ESP32, the Arduino loop is a task itself
     // We can keep this empty since our work is done in FreeRTOS tasks
     vTaskDelay(1000 / portTICK_PERIOD_MS);
 }