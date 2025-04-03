// IO.h
#ifndef IO_H
#define IO_H

#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

class IO {
public:
    // Output modes
    enum OutputMode {
        SERIAL_MODE,
        LCD_MODE,
        SERIAL_LCD_MODE
    };
    
    static void init(OutputMode mode);
    
    // LCD functions
    static void lcd_print(const char* text);
    static void lcd_print(const String& text);
    static void lcd_setCursor(int col, int row);
    static void lcd_clear();
    
    // Serial redirection for printf
    static int printf(const char* format, ...);
    
private:
    static OutputMode currentMode;
    static LiquidCrystal_I2C lcd;
};

#endif

