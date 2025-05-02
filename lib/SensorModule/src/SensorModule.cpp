// SensorModule.cpp
#include "SensorModule.h"

SensorModule::SensorModule() {
    usesDigitalSensor = false;  // Set to true if using digital sensor
}

void SensorModule::init() {  // Added SensorModule:: to define this as class method
    // Initialize the sensor pins
    pinMode(analogPin, INPUT);
    
    if (usesDigitalSensor) {
        pinMode(digitalPin, INPUT);
    }
    
    // Any additional sensor-specific initialization
    delay(100);  // Some sensors need time to stabilize
    
    Serial.println("Sensor module initialized");
}

RawSensorData SensorModule::readSensor() {
    RawSensorData data;
    
    // Read analog value
    data.analogValue = analogRead(analogPin);
    
    // Read digital state if applicable
    if (usesDigitalSensor) {
        data.digitalState = digitalRead(digitalPin);
    } else {
        data.digitalState = false;
    }
    
    // Record acquisition timestamp
    data.timestamp = millis();
    
    return data;
}