/**
 * @file lcd_i2c_driver.c
 * @brief Implementation of low-level I2C LCD driver
 */

 #include "lcd_driver.h"
 #include <Arduino.h>
 #include <Wire.h>
 #include <LiquidCrystal_I2C.h>
 #include <stdarg.h>
 #include <stdio.h>
 
 // LCD object
 static LiquidCrystal_I2C *_lcd = NULL;
 static lcd_i2c_config_t _config;
 
 bool lcd_i2c_init(const lcd_i2c_config_t *config) {
     if (!config) return false;
     
     // Store configuration
     _config = *config;
     
     // Initialize I2C with custom pins if provided
     if (_config.sda_pin != 0 && _config.scl_pin != 0) {
         Wire.begin(_config.sda_pin, _config.scl_pin);
     } else {
         Wire.begin(); // Use default pins
     }
     
     // Create LCD object with I2C address, columns and rows
     _lcd = new LiquidCrystal_I2C(_config.i2c_addr, _config.columns, _config.rows);
     
     // Initialize the LCD
     _lcd->init();
     _lcd->backlight();
     
     // Initial clear
     lcd_i2c_clear();
     
     return true;
 }
 
 void lcd_i2c_clear(void) {
     if (_lcd) {
         _lcd->clear();
     }
 }
 
 void lcd_i2c_set_cursor(uint8_t column, uint8_t row) {
     if (_lcd) {
         _lcd->setCursor(column, row);
     }
 }
 
 void lcd_i2c_print(const char *text) {
     if (_lcd && text) {
         _lcd->print(text);
     }
 }
 
 void lcd_i2c_printf(const char *format, ...) {
     if (!_lcd || !format) return;
     
     char buffer[_config.columns + 1];
     va_list args;
     
     va_start(args, format);
     vsnprintf(buffer, sizeof(buffer), format, args);
     va_end(args);
     
     _lcd->print(buffer);
 }
 
 void lcd_i2c_backlight(bool on) {
     if (_lcd) {
         if (on) {
             _lcd->backlight();
         } else {
             _lcd->noBacklight();
         }
     }
 }
 
 void lcd_i2c_create_char(uint8_t location, const uint8_t charmap[]) {
     if (_lcd && location < 8) {
         _lcd->createChar(location, (uint8_t*)charmap);
     }
 }