#pragma once
#include "Arduino.h"
#include "SoundSensor.h"

// Sound detection threshold
constexpr float SOUND_THRESHOLD_VOLTAGE = 2.9; // Voltage threshold for detection
constexpr int DEBOUNCE_TIME_MS = 50;           // Debounce time in ms

enum SystemState {
  IDLE,
  SOUND_DETECTED,
  WARNING,
  ERROR
};

struct SystemStatus {
  SystemState currentState;
  unsigned long lastStateChangeTime;
  int detectionCount;
  bool thresholdExceeded;
};

class SignalManager {
public:
  SignalManager();
  void init();
  void processSignals(const SensorData& data);
  SensorData getCurrentData();
  SystemStatus getSystemStatus();
  bool isSoundDetected();
  
private:
  SensorData currentData;
  SystemStatus systemStatus;
  int lastDigitalState;
  unsigned long lastDebounceTime;
  
  // Process analog signal
  void processAnalogSignal(float voltage);
  
  // Process digital signal with debouncing
  void processDigitalSignal(int digitalValue);
};