#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LcdDisplay {
private:
    LiquidCrystal_I2C* lcd;
    uint8_t cols;
    uint8_t rows;

public:
    /**
     * Constructor for LcdDisplay
     * @param address I2C address of the LCD (default 0x27)
     * @param columns Number of LCD columns (default 16)
     * @param rows Number of LCD rows (default 2)
     */
    LcdDisplay(uint8_t address = 0x27, uint8_t columns = 16, uint8_t rows = 2);
    
    /**
     * Initialize the LCD
     */
    void begin();
    
    /**
     * Clear the LCD display
     */
    void clear();
    
    /**
     * Display a message on the LCD
     * @param message Text to display
     * @param row Row position (0-based)
     * @param col Column position (0-based)
     */
    void displayMessage(const char* message, uint8_t row = 0, uint8_t col = 0);
    
    /**
     * Display relay status on the LCD
     * @param isOn True if relay is on, false if off
     */
    void displayRelayStatus(bool isOn);
    
    /**
     * Display command confirmation
     * @param command Command string
     * @param success True if command was successful
     */
    void displayCommandConfirmation(const char* command, bool success);
};

#endif // LCD_DISPLAY_H