#pragma once
#include "Arduino.h"

// Constants for sensor pins
constexpr int ANALOG_SOUND_PIN = 35;   // Analog sound sensor input (AO)
constexpr int DIGITAL_SOUND_PIN = 34;  // Digital sound sensor input (DO)

struct SensorData {
  int analogValue;     // Raw ADC value
  float voltage;       // Converted voltage
  int digitalValue;    // Digital trigger state
  unsigned long timestamp; // Timestamp of acquisition
};

class SoundSensor {
public:
  SoundSensor();
  void init();
  SensorData readSensor();
  
private:
  // Analog to voltage conversion
  float convertToVoltage(int adcValue);
};