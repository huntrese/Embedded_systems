#include "SensorReader.h"

SensorReader::SensorReader(uint8_t pin) : _pin(pin) {}

void SensorReader::init() {
    pinMode(_pin, INPUT);
    // Configure ADC settings if necessary (e.g., attenuation, resolution)
    // analogReadResolution(12); // Default on ESP32
    // analogSetAttenuation(ADC_11db); // Check datasheet for voltage range vs attenuation
    Serial.printf("SensorReader initialized for pin %d\n", _pin);
}

int SensorReader::readRawValue() {
    return analogRead(_pin);
}