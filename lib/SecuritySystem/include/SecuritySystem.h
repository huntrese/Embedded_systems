// SecuritySystem.h - Simplified security code checker
#pragma once

#include <Arduino.h>

class SecuritySystem {
public:
    // Initialize security system
    static void init(uint8_t greenLed, uint8_t redLed);
    
    // Process input and update system state
    static void update();

private:
    static const uint8_t CODE_LENGTH = 4;
    static char enteredCode[CODE_LENGTH + 1];
    static char correctCode[CODE_LENGTH + 1];
    static uint8_t codeIndex;
    static bool programMode;
    static uint8_t greenLed;
    static uint8_t redLed;
    
    // Reset input state
    static void reset();
    
    // Check entered code
    static void checkCode();
};