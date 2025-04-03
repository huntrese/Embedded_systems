// SensorModule.h
#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include "Arduino.h"

// Structure to hold raw sensor data
struct RawSensorData {
    int analogValue;      // ADC reading from analog sensor
    bool digitalState;    // Digital state for digital sensors
    uint32_t timestamp;   // Timestamp of acquisition
};

class SensorModule {
public:
    SensorModule();
    void init();
    RawSensorData readSensor();
private:
    const int analogPin = A0;     // Analog input pin
    const int digitalPin = 14;    // Digital input pin
    bool usesDigitalSensor = true;       // Flag to indicate sensor type
};

#endif

