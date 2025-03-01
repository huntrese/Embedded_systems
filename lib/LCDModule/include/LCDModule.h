#pragma once
#include <LiquidCrystal_I2C.h>

class LCDModule {
public:
    static void init(uint8_t address, uint8_t cols, uint8_t rows);
    static void clear();
    static void print(uint8_t col, uint8_t row, const char* message);
    static void handleStdioChar(char c);
    
private:
    static LiquidCrystal_I2C lcd;
    static uint8_t currentCol;
    static uint8_t currentRow;
    static uint8_t cols;
    static uint8_t rows;
    
    static void updateCursor();
};