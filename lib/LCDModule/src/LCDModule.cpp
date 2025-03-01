#include "LCDModule.h"

LiquidCrystal_I2C LCDModule::lcd(0x27, 16, 2);
uint8_t LCDModule::currentCol = 0;
uint8_t LCDModule::currentRow = 0;
uint8_t LCDModule::cols = 16;
uint8_t LCDModule::rows = 2;

void LCDModule::init(uint8_t address, uint8_t columns, uint8_t rows) {
    cols = columns;
    rows = rows;
    lcd = LiquidCrystal_I2C(address, cols, rows);
    lcd.init();
    lcd.backlight();
    clear();
}

void LCDModule::clear() {
    lcd.clear();
    currentCol = 0;
    currentRow = 0;
}

void LCDModule::print(uint8_t col, uint8_t row, const char* message) {
    lcd.setCursor(col, row);
    lcd.print(message);
    currentCol = col + strlen(message);
    currentRow = row;
}

void LCDModule::handleStdioChar(char c) {
    if(c == '\n') {
        currentRow++;
        currentCol = 0;
    }
    else if(c == '\r') {
        // Ignore carriage return
        return;
    }
    else {
        lcd.write(c);
        currentCol++;
    }

    if(currentCol >= cols) {
        currentCol = 0;
        currentRow++;
    }

    if(currentRow >= rows) {
        currentRow = 0;
    }
    
    updateCursor();
}

void LCDModule::updateCursor() {
    lcd.setCursor(currentCol, currentRow);
}