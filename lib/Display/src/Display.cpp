#include "Display.h"

LcdDisplay::LcdDisplay(uint8_t address, uint8_t columns, uint8_t rows) {
    lcd = new LiquidCrystal_I2C(address, columns, rows);
    cols = columns;
    this->rows = rows;
}

void LcdDisplay::begin() {
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Relay Control");
    lcd->setCursor(0, 1);
    lcd->print("Status: OFF");
}

void LcdDisplay::clear() {
    lcd->clear();
}

void LcdDisplay::displayMessage(const char* message, uint8_t row, uint8_t col) {
    lcd->setCursor(col, row);
    lcd->print(message);
}

void LcdDisplay::displayRelayStatus(bool isOn) {
    lcd->setCursor(0, 1);
    lcd->print("Status: ");
    if (isOn) {
        lcd->print("ON ");
    } else {
        lcd->print("OFF");
    }
}

void LcdDisplay::displayCommandConfirmation(const char* command, bool success) {
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Command: ");
    lcd->print(command);
    lcd->setCursor(0, 1);
    if (success) {
        lcd->print("Status: Success");
    } else {
        lcd->print("Status: Failed");
    }
    
    // After 2 seconds, return to displaying relay status
    delay(2000);
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Relay Control");
}