// Main sketch file (SecuritySystemMain.ino)
#include "IO.h"
#include "SecuritySystem.h"

// Hardware configuration
#define KEYPAD_PIN 4       // First pin for keypad (needs 8 consecutive pins)
#define LCD_ADDRESS 0x27   // I2C address for LCD
#define LCD_COLS 16        // LCD columns
#define LCD_ROWS 2         // LCD rows
#define GREEN_LED 13       // Success LED
#define RED_LED 12         // Error LED

void setup() {
    // Choose IO mode - Serial or LCD/Keypad
    IO::init(IO::LCD_KEYPAD_MODE);
    
    // For LCD/Keypad mode, initialize hardware
    IO::initLCD(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
    IO::initKeypad(KEYPAD_PIN);
    
    // Initialize security system
    SecuritySystem::init(GREEN_LED, RED_LED);
}

void loop() {
    // Run security system update
    SecuritySystem::update();
    
    // Small delay for stability
    delay(50);
}