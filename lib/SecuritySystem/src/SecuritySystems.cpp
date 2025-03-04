// SecuritySystem.cpp - Implementation of security system
#include "SecuritySystem.h"
#include "IO.h"
#include "LedManager.h"


// Initialize static variables
char SecuritySystem::enteredCode[CODE_LENGTH + 1] = "";
char SecuritySystem::correctCode[CODE_LENGTH + 1] = "1234"; // Default code
uint8_t SecuritySystem::codeIndex = 0;
bool SecuritySystem::programMode = false;
uint8_t SecuritySystem::greenLed = 0;
uint8_t SecuritySystem::redLed = 0;

void SecuritySystem::init(uint8_t greenLedPin, uint8_t redLedPin) {
    // Save pin numbers
    greenLed = greenLedPin;
    redLed = redLedPin;
    
    // Setup LED pins
    pinMode(greenLed, OUTPUT);
    pinMode(redLed, OUTPUT);
    
    // Turn off LEDs
    LedManager::off(greenLed);
    LedManager::off(redLed);
    
    // Show welcome message
    printf("Security System\nEnter code: ");
}

void SecuritySystem::update() {
    // Check for key press
    if (stdin->flags & _FDEV_SETUP_READ) {
        int c = getchar();
        if (c != EOF) {
            // Process key
            char key = (char)c;
            
            // Check for numeric and special keys
            if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'D') || key == '*' || key == '#') {
                if (codeIndex < CODE_LENGTH) {
                    enteredCode[codeIndex++] = key;
                    
                    // Check if code is complete
                    if (codeIndex >= CODE_LENGTH) {
                        checkCode();
                    }
                }
            }
        }
    }
    
    // Check for program mode (hold D key)
    if (IO::isKeyHeld('D') && !programMode) {
        programMode = true;
        reset();
        printf("\nNew Code: ");
    }
}

void SecuritySystem::reset() {
    // Reset input state
    codeIndex = 0;
    memset(enteredCode, 0, CODE_LENGTH + 1);
    
    // Turn off LEDs
    LedManager::off(greenLed);
    LedManager::off(redLed);
    
    // Clear display properly based on mode
    if (IO::currentMode == IO::LCD_KEYPAD_MODE) {
        IO::clearLCD();
        printf("Enter code: ");
    } else {
        printf("\nEnter code: ");
    }
}

void SecuritySystem::checkCode() {
    // Null-terminate code
    enteredCode[codeIndex] = '\0';
    IO::clearLCD();
    if (programMode) {
        // Save new code
        strcpy(correctCode, enteredCode);
        printf("\nCode updated!");
        LedManager::on(greenLed);
        delay(2000);
        LedManager::off(greenLed);
        programMode = false;
    } else {
        // Check code
        if (strcmp(enteredCode, correctCode) == 0) {
            printf("\nAccess Granted!");
            LedManager::on(greenLed);
            delay(2000);
            LedManager::off(greenLed);
        } else {
            printf("\nAccess Denied!");
            LedManager::on(redLed);
            delay(2000);
            LedManager::off(redLed);
        }
    }
    
    // Reset for next entry
    reset();
}