// SignalManager.cpp
#include "SignalManager.h"

SignalManager::SignalManager() {
  // Initialize member variables
  lastDigitalState = LOW;
  lastDebounceTime = 0;
  
  // Initialize system status
  systemStatus.currentState = IDLE;
  systemStatus.lastStateChangeTime = 0;
  systemStatus.detectionCount = 0;
  systemStatus.thresholdExceeded = false;
}

void SignalManager::init() {
  // Perform initialization
  printf("Signal Manager Initialized\n");
}

void SignalManager::processSignals(const SensorData& data) {
  // Store current data
  currentData = data;
  
  // Process analog signal
  processAnalogSignal(data.voltage);
  
  // Process digital signal with debouncing
  processDigitalSignal(data.digitalValue);
}

void SignalManager::processAnalogSignal(float voltage) {
  // Check if voltage exceeds threshold
  bool exceeded = voltage > SOUND_THRESHOLD_VOLTAGE;
  
  // Update threshold exceeded status
  if (exceeded != systemStatus.thresholdExceeded) {
    systemStatus.thresholdExceeded = exceeded;
    systemStatus.lastStateChangeTime = millis();
    
    // If exceeded, update system state to WARNING
    if (exceeded) {
      systemStatus.currentState = WARNING;
      printf("⚠️ Warning: Sound threshold exceeded (%.2fV)\n", voltage);
    } else if (systemStatus.currentState == WARNING) {
      // Return to IDLE if we were in WARNING state
      systemStatus.currentState = IDLE;
    }
  }
}

void SignalManager::processDigitalSignal(int digitalValue) {
  // Implement debouncing logic
  if (digitalValue != lastDigitalState) {
    lastDebounceTime = millis();
  }
  
  // Check if debounce time has passed
  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME_MS) {
    // If state has actually changed
    if (digitalValue == HIGH && lastDigitalState == LOW) {
      // Sound detected
      systemStatus.currentState = SOUND_DETECTED;
      systemStatus.lastStateChangeTime = millis();
      systemStatus.detectionCount++;
      
      printf("🔉 Sound Detected! Count: %d\n", systemStatus.detectionCount);
    } 
    else if (digitalValue == LOW && lastDigitalState == HIGH) {
      // Return to IDLE or WARNING depending on threshold
      if (systemStatus.thresholdExceeded) {
        systemStatus.currentState = WARNING;
      } else {
        systemStatus.currentState = IDLE;
      }
    }
  }
  
  // Update last state
  lastDigitalState = digitalValue;
}

SensorData SignalManager::getCurrentData() {
  return currentData;
}

SystemStatus SignalManager::getSystemStatus() {
  return systemStatus;
}

bool SignalManager::isSoundDetected() {
  return (systemStatus.currentState == SOUND_DETECTED);
}