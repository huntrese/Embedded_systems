// IO.cpp - Implementation of IO functions
#include "IO.h"
#include "Logger.h"

// Initialize static variables
LiquidCrystal_I2C IO::lcd(0x27, 16, 2);
uint8_t IO::lcdCols = 16;
uint8_t IO::lcdRows = 2;
uint8_t IO::cursorCol = 0;
uint8_t IO::cursorRow = 0;
uint8_t IO::keypadPin = 4;
char IO::lastKey = 0;
unsigned long IO::keyPressTime = 0;
IO::IOMode IO::currentMode = IO::SERIAL_MODE;

const char IO::keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', '\n'}
};

const unsigned long IO::HOLD_THRESHOLD = 1500; // 1.5 seconds

// Serial output function
int IO::serial_putchar(char c, FILE* f) {
    Serial.write(c);
    return 0;
}

// Serial input function
int IO::serial_getchar(FILE* f) {
    while (Serial.available() <= 0);
    char c = Serial.read();
    Serial.write(c);
    return c;
}

// LCD output function
int IO::lcd_putchar(char c, FILE* f) {
    if (c == '\n') {
        // New line
        cursorCol = 0;
        cursorRow = (cursorRow + 1) % lcdRows;
        lcd.setCursor(cursorCol, cursorRow);
    } else if (c == '\r') {
        // Carriage return
        cursorCol = 0;
        lcd.setCursor(cursorCol, cursorRow);
    } else {
        // Normal character
        lcd.write(c);
        cursorCol++;
        
        // Handle wrapping
        if (cursorCol >= lcdCols) {
            cursorCol = 0;
            cursorRow = (cursorRow + 1) % lcdRows;
            lcd.setCursor(cursorCol, cursorRow);
        }
    }
    return 0;
}

// Keypad input function
int IO::keypad_getchar(FILE* f) {
    char key = 0;
    
    // Wait until key pressed
    while (key == 0) {
        // Scan keypad
        for (int row = 0; row < 4; row++) {
            // Set row pin as OUTPUT and LOW
            pinMode(keypadPin + row, OUTPUT);
            digitalWrite(keypadPin + row, LOW);
            
            // Check each column
            for (int col = 0; col < 4; col++) {
                pinMode(keypadPin + 4 + col, INPUT_PULLUP);
                delay(5);
                
                if (digitalRead(keypadPin + 4 + col) == LOW) {
                    // Key detected
                    key = keys[row][col];
                    lastKey = key;
                    keyPressTime = millis();
                    
                    // Show key on LCD
                    lcd_putchar(key, f);
                    
                    // Wait for release
                    while (digitalRead(keypadPin + 4 + col) == LOW) {
                        delay(10);
                    }
                    delay(50); // Debounce
                    Logger::printf("key %c",key);
                }
            }
            
            // Reset row pin
            pinMode(keypadPin + row, INPUT);
        }
        delay(10);
    }
    
    return key;
}

void IO::init(IOMode mode) {
    currentMode = mode;

    if (mode == SERIAL_MODE) {
        Serial.begin(115200);  // ESP32 supports higher speeds, 115200 is more standard
        delay(100);
    }

    #ifndef ESP32  // AVR-based systems
        // Setup stdin/stdout using fdev_setup_stream for AVR
        static FILE stream;
        if (mode == SERIAL_MODE) {
            fdev_setup_stream(&stream, serial_putchar, serial_getchar, _FDEV_SETUP_RW);
        } else {
            fdev_setup_stream(&stream, lcd_putchar, keypad_getchar, _FDEV_SETUP_RW);
        }
        stdin = stdout = &stream;
    #else
        // ESP32 uses built-in Serial functions, no need for fdev_setup_stream
        if (mode == SERIAL_MODE) {
            Serial.println("ESP32 Serial Mode Active");
        } else {
            Serial.println("ESP32 LCD/Keypad Mode Active");
        }
    #endif
}


void IO::initLCD(uint8_t address, uint8_t cols, uint8_t rows) {
    lcd = LiquidCrystal_I2C(address, cols, rows);
    lcdCols = cols;
    lcdRows = rows;
    
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.home();
    
    cursorCol = 0;
    cursorRow = 0;
}

void IO::initKeypad(uint8_t startPin) {
    keypadPin = startPin;
    
    // Setup pins
    for (int i = 0; i < 8; i++) {
        pinMode(keypadPin + i, INPUT_PULLUP);
    }
}

void IO::clearLCD() {
    if (currentMode == LCD_KEYPAD_MODE) {
        lcd.clear();
        lcd.home();
        cursorCol = 0;
        cursorRow = 0;
    }
}

void IO::clearInputBuffer() {
    if (currentMode == SERIAL_MODE) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

bool IO::isKeyHeld(char key) {
    if (currentMode != LCD_KEYPAD_MODE) return false;
    
    if (lastKey == key && (millis() - keyPressTime) > HOLD_THRESHOLD) {
        return true;
    }
    return false;
}