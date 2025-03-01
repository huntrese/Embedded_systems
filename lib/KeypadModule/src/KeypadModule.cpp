#include "KeypadModule.h"

uint8_t KeypadModule::rowPins[4];
uint8_t KeypadModule::colPins[4];
const char KeypadModule::keymap[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
unsigned long KeypadModule::lastKeyTime = 0;
char KeypadModule::lastKey = '\0';

void KeypadModule::init(uint8_t rowP[4], uint8_t colP[4]) {
    memcpy(rowPins, rowP, sizeof(rowPins));
    memcpy(colPins, colP, sizeof(colPins));
    
    for(uint8_t i = 0; i < 4; i++) {
        pinMode(rowPins[i], INPUT_PULLUP);
        pinMode(colPins[i], OUTPUT);
        digitalWrite(colPins[i], HIGH);
    }
}

char KeypadModule::getKey() {
    static unsigned long lastKeyTime = 0;
    char key = '\0';
    
    for(uint8_t c = 0; c < 4; c++) {
        digitalWrite(colPins[c], LOW);
        for(uint8_t r = 0; r < 4; r++) {
            if(digitalRead(rowPins[r]) == LOW) {
                key = keymap[r][c];
            }
        }
        digitalWrite(colPins[c], HIGH);
        delay(1);
    }

    if(key != '\0' && (millis() - lastKeyTime) > debounceTime) {
        lastKeyTime = millis();
        lastKey = key;
        return key;
    }
    
    return '\0';
}