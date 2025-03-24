#pragma once
#include "Arduino.h"
#include "IO.h"
#include "SoundSensor.h"
#include "SignalManager.h"

// LCD configuration
constexpr int LCD_ADDRESS = 0x27;  // I2C address for LCD
constexpr int LCD_COLS = 16;       // Number of columns on LCD
constexpr int LCD_ROWS = 2;        // Number of rows on LCD

class DisplayManager {
public:
  DisplayManager();
  void init();
  void updateDisplay(const SensorData& data, const SystemStatus& status);
  
private:
  unsigned long lastStdioDisplayTime;
  const unsigned long stdioUpdateInterval = 1000; // Update stdio every 1 second
  
  // Helper functions
  void displayOnLCD(const SensorData& data, const SystemStatus& status);
  void displayOnSerial(const SensorData& data, const SystemStatus& status);
  
  // System state to string
  const char* getStateString(SystemState state);
};