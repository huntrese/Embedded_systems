/**
 * @file status_reporter.c
 * @brief Implementation of high-level status reporting module
 */

 #include "status_reporter.h"
 #include "motor_control.h"
 #include "lcd_driver.h"
 #include <Arduino.h>
 #include <freertos/FreeRTOS.h>
 #include <freertos/task.h>
 
 // Status update interval in milliseconds
 #define STATUS_UPDATE_INTERVAL 1000
 
 // Last reported values for change detection
 static int8_t _last_reported_power = 0;
 static int8_t _last_reported_direction = 0;
 
 bool status_reporter_init(void) {
     // Initialize LCD with I2C
     lcd_i2c_config_t lcd_config = {
         .i2c_addr = 0x27,  // Default I2C address for most PCF8574 backpacks
         .columns = 16,     // Common 16x2 LCD
         .rows = 2,
         .sda_pin = 21,     // SDA pin (adjust according to your board)
         .scl_pin = 22      // SCL pin (adjust according to your board)
     };
     
     bool lcd_ok = lcd_i2c_init(&lcd_config);
     
     if (lcd_ok) {
         // Display initial message
         lcd_i2c_clear();
         lcd_i2c_set_cursor(0, 0);
         lcd_i2c_print("DC Motor Control");
         lcd_i2c_set_cursor(0, 1);
         lcd_i2c_print("Power: 0%");
     }
     
     return lcd_ok;
 }
 
 void status_report_immediate(void) {
     int8_t current_power = motor_get_power();
     int8_t current_direction = motor_get_direction();
     
     // Only update if values changed
     if (current_power != _last_reported_power || 
         current_direction != _last_reported_direction) {
         
         // Update last reported values
         _last_reported_power = current_power;
         _last_reported_direction = current_direction;
         
         // Report to serial
         Serial.print("Motor status: ");
         if (current_power == 0) {
             Serial.println("STOPPED");
         } else {
             Serial.print(current_direction > 0 ? "FORWARD" : "REVERSE");
             Serial.print(", Power: ");
             Serial.print(abs(current_power));
             Serial.println("%");
         }
         
         // Report to LCD
         lcd_i2c_set_cursor(0, 1);
         
         if (current_power == 0) {
             lcd_i2c_print("Power: STOPPED   ");
         } else {
             char direction = current_direction > 0 ? 'F' : 'R';
             lcd_i2c_printf("Power: %c %3d%%    ", direction, abs(current_power));
         }
     }
 }
 
 void status_reporter_task(void *pvParameters) {
     for (;;) {
         status_report_immediate();
         vTaskDelay(pdMS_TO_TICKS(STATUS_UPDATE_INTERVAL));
     }
 }