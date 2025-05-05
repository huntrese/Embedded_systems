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
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// LCD object
static LiquidCrystal_I2C *_lcd = NULL;
static lcd_i2c_config_t _config;

// Mutex for LCD access
static SemaphoreHandle_t lcd_mutex = NULL;

bool lcd_i2c_init(const lcd_i2c_config_t *config) {
    if (!config) return false;

    // Create the mutex if it doesn't exist
    if (lcd_mutex == NULL) {
        lcd_mutex = xSemaphoreCreateMutex();
        if (lcd_mutex == NULL) {
            return false; // Failed to create mutex
        }
    }

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
    _lcd->backlight(); // Ensure backlight is turned on

    // Initial clear
    lcd_i2c_clear();

    // Display a test message to confirm initialization
    lcd_i2c_set_cursor(0, 0);
    lcd_i2c_print("LCD Initialized");

    return true;
}

void lcd_i2c_clear(void) {
    if (_lcd) {
        if (xSemaphoreTake(lcd_mutex, portMAX_DELAY)) {
            _lcd->clear();
            xSemaphoreGive(lcd_mutex);
        }
    }
}

void lcd_i2c_set_cursor(uint8_t column, uint8_t row) {
    if (_lcd) {
        if (xSemaphoreTake(lcd_mutex, portMAX_DELAY)) {
            _lcd->setCursor(column, row);
            xSemaphoreGive(lcd_mutex);
        }
    }
}

void lcd_i2c_print(const char *text) {
    if (_lcd && text) {
        if (xSemaphoreTake(lcd_mutex, portMAX_DELAY)) {
            _lcd->print(text);
            xSemaphoreGive(lcd_mutex);
        }
    }
}

void lcd_i2c_printf(const char *format, ...) {
    if (!_lcd || !format) return;

    if (xSemaphoreTake(lcd_mutex, portMAX_DELAY)) {
        char buffer[_config.columns + 1];
        va_list args;

        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        _lcd->print(buffer);
        xSemaphoreGive(lcd_mutex);
    }
}

void lcd_i2c_backlight(bool on) {
    if (_lcd) {
        if (xSemaphoreTake(lcd_mutex, portMAX_DELAY)) {
            if (on) {
                _lcd->backlight();
            } else {
                _lcd->noBacklight();
            }
            xSemaphoreGive(lcd_mutex);
        }
    }
}

void lcd_i2c_create_char(uint8_t location, const uint8_t charmap[]) {
    if (_lcd && location < 8) {
        if (xSemaphoreTake(lcd_mutex, portMAX_DELAY)) {
            _lcd->createChar(location, (uint8_t*)charmap);
            xSemaphoreGive(lcd_mutex);
        }
    }
}