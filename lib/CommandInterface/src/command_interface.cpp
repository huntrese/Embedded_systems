/**
 * @file command_interface.c
 * @brief Implementation of high-level command interface
 */

 #include "command_interface.h"
 #include "motor_control.h"
 #include "status_reporter.h"
 #include <Arduino.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 
 #define CMD_BUFFER_SIZE 64
 #define CMD_MAX_ARGS 5
 
 bool command_interface_init(void) {
     // Initialize serial for command input
     Serial.begin(115200);
     printf("\nDC Motor Control System");
     printf("\nCommands:");
     printf("\n  motor set [-100..100] - Set motor power");
     printf("\n  motor stop - Stop motor");
     printf("\n  motor max - Maximum power");
     printf("\n  motor inc - Increase power");
     printf("\n  motor dec - Decrease power\n");
     
     return true;
 }
 
 bool command_process(void) {
     static char buffer[CMD_BUFFER_SIZE];
     static int idx = 0;
     bool cmd_ready = false;
     
     // Read available data from serial
     while (Serial.available() && idx < CMD_BUFFER_SIZE - 1) {
         char c = Serial.read();
         
         // Echo character
         Serial.write(c);
         
         if (c == '\r' || c == '\n') {
             if (idx > 0) {
                 buffer[idx] = '\0';
                 cmd_ready = true;
                 Serial.println();
                 break;
             }
         } else if (c == 8 || c == 127) { // Backspace/Delete
             if (idx > 0) {
                 idx--;
                 Serial.print("\b \b"); // Erase character on terminal
             }
         } else {
             buffer[idx++] = c;
         }
     }
     
     if (!cmd_ready) return false;
     
     // Parse command
     char *token;
     char *args[CMD_MAX_ARGS];
     int arg_count = 0;
     
     token = strtok(buffer, " \t");
     while (token != NULL && arg_count < CMD_MAX_ARGS) {
         args[arg_count++] = token;
         token = strtok(NULL, " \t");
     }
     
     // Process command
     if (arg_count >= 2 && strcmp(args[0], "motor") == 0) {
         // Motor control commands
         if (strcmp(args[1], "set") == 0 && arg_count == 3) {
             // Set motor power
             int power = atoi(args[2]);
             if (motor_set_power(power)) {
                 status_report_immediate();
             } else {
                 printf("\nError: Power value must be between -100 and 100");
             }
         } 
         else if (strcmp(args[1], "stop") == 0) {
             // Stop motor
             motor_stop();
             printf("\nMotor stopped");
             status_report_immediate();
         }
         else if (strcmp(args[1], "max") == 0) {
             // Max power
             motor_set_max();
             printf("\nMotor set to maximum power");
             status_report_immediate();
         }
         else if (strcmp(args[1], "inc") == 0) {
             // Increment power
             int new_power = motor_increment();
             
             // Additional behavior - show message at max power
             if (abs(new_power) == 100) {
                 printf("\nMaximum power reached!");
             } else {
                 Serial.print("Power increased to ");
                 Serial.println(new_power);
             }
             
             status_report_immediate();
         }
         else if (strcmp(args[1], "dec") == 0) {
             // Decrement power
             int new_power = motor_decrement();
             
             // Additional behavior - show message at min power
             if (new_power == 0) {
                 printf("\nMinimum power reached - motor stopped");
             } else {
                 Serial.print("Power decreased to ");
                 Serial.println(new_power);
             }
             
             status_report_immediate();
         }
         else {
             printf("\nUnknown motor command");
         }
     } else {
         printf("\nUnknown command");
     }
     
     // Reset buffer for next command
     idx = 0;
     
     return true;
 }
 
 void command_task(void *pvParameters) {
     for (;;) {
         command_process();
         vTaskDelay(pdMS_TO_TICKS(50)); // 50ms polling interval
     }
 }