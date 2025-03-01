#pragma once
#include <Arduino.h>

class KeypadModule {
public:
    static void init(uint8_t rowPins[4], uint8_t colPins[4]);
    static char getKey();
    static bool isKeyHeld(char key);
    static unsigned long getKeyHoldDuration(char key);
    
private:
    static uint8_t rowPins[4];
    static uint8_t colPins[4];
    static const char keymap[4][4];
    static unsigned long lastKeyTime;
    static char lastKey;
    static const unsigned long debounceTime = 50;
};