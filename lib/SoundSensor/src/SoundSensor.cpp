// SoundSensor.cpp
#include "SoundSensor.h"

SoundSensor::SoundSensor() {
  // Constructor
}

void SoundSensor::init() {
  // Configure pins
  pinMode(DIGITAL_SOUND_PIN, INPUT);
  
  // Configure ADC resolution (ESP32 specific)
  analogReadResolution(12); // 12-bit ADC (0-4095)
  
  // Print initialization message
  printf("Sound Sensor Initialized\n");
  printf("Analog Pin: %d, Digital Pin: %d\n", ANALOG_SOUND_PIN, DIGITAL_SOUND_PIN);
}

SensorData SoundSensor::readSensor() {
  SensorData data;
  
  // Read sensor values
  data.analogValue = analogRead(ANALOG_SOUND_PIN);
  data.voltage = convertToVoltage(data.analogValue);
  data.digitalValue = digitalRead(DIGITAL_SOUND_PIN);
  data.timestamp = millis();
  
  return data;
}

float SoundSensor::convertToVoltage(int adcValue) {
  // Convert ADC value to voltage (3.3V reference with 12-bit ADC)
  return adcValue * (3.3 / 4095.0);
}