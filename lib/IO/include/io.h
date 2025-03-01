// IO.h - Simplified IO handling for Serial and LCD/Keypad
#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>

class IO {
public:
    enum IOMode {
        SERIAL_MODE,
        LCD_KEYPAD_MODE
    };

    // Initialize IO system
    static void init(IOMode mode = SERIAL_MODE);
    
    // Initialize LCD
    static void initLCD(uint8_t address, uint8_t cols, uint8_t rows);
    
    // Initialize keypad
    static void initKeypad(uint8_t startPin);
    
    // Clear LCD screen
    static void clearLCD();
    
    // Clear input buffer
    static void clearInputBuffer();
    
    // Check if a key is being held
    static bool isKeyHeld(char key);
    
    // Current IO mode
    static IOMode currentMode;

private:
    // IO functions
    static int serial_putchar(char c, FILE* f);
    static int serial_getchar(FILE* f);
    static int lcd_putchar(char c, FILE* f);
    static int keypad_getchar(FILE* f);
    
    // LCD variables
    static LiquidCrystal_I2C lcd;
    static uint8_t lcdCols;
    static uint8_t lcdRows;
    static uint8_t cursorCol;
    static uint8_t cursorRow;
    
    // Keypad variables
    static uint8_t keypadPin;
    static char lastKey;
    static unsigned long keyPressTime;
    static const char keys[4][4];
    static const unsigned long HOLD_THRESHOLD;
};