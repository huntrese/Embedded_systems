#pragma once

#include "Arduino.h"
#include "config.h"

class SensorReader {
public:
    SensorReader(uint8_t pin);
    void init();
    int readRawValue(); // Reads the ADC value

private:
    uint8_t _pin;
};