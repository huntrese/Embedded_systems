// IO.cpp
#include "IO.h"
#include <stdarg.h>

// Initialize static members
IO::OutputMode IO::currentMode = IO::SERIAL_MODE;
LiquidCrystal_I2C IO::lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows

void IO::init(OutputMode mode) {
    currentMode = mode;
    
    if (mode == LCD_MODE || mode == SERIAL_LCD_MODE) {
        lcd.init();
        lcd.backlight();
        lcd.clear();
        lcd.print("Initializing...");
    }
    
    if (mode == SERIAL_MODE || mode == SERIAL_LCD_MODE) {
        // Serial is initialized in setup()
    }
}

void IO::lcd_print(const char* text) {
    if (currentMode == LCD_MODE || currentMode == SERIAL_LCD_MODE) {
        lcd.print(text);
    }
}

void IO::lcd_print(const String& text) {
    if (currentMode == LCD_MODE || currentMode == SERIAL_LCD_MODE) {
        lcd.print(text);
    }
}

void IO::lcd_setCursor(int col, int row) {
    if (currentMode == LCD_MODE || currentMode == SERIAL_LCD_MODE) {
        lcd.setCursor(col, row);
    }
}

void IO::lcd_clear() {
    if (currentMode == LCD_MODE || currentMode == SERIAL_LCD_MODE) {
        lcd.clear();
    }
}

int IO::printf(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    int result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (currentMode == SERIAL_MODE || currentMode == SERIAL_LCD_MODE) {
        Serial.print(buffer);
    }
    
    return result;
}