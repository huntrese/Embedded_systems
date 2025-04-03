#pragma once
#include "config.h" // For ProcessedData struct
#include "Arduino.h"
#include "IO.h"
#include "SoundSensor.h"
#include "SignalManager.h"

// LCD configuration
constexpr int LCD_ADDRESS = 0x27; // I2C address for LCD
constexpr int LCD_COLS = 16;      // Number of columns on LCD
constexpr int LCD_ROWS = 2;       // Number of rows on LCD
// --- Pin Definitions ---
#define LDR_PIN 34 // ADC1_CH6 on many ESP32 boards (Ensure it's an ADC pin)

// --- ADC Configuration ---
#define ADC_MAX_VALUE 4095.0  // For 12-bit ADC (ESP32 default)
#define REFERENCE_VOLTAGE 3.3 // ESP32 operating voltage

// --- Task Configuration ---
#define ACQUISITION_TASK_STACK_SIZE 4096 // Increased stack for filtering buffers
#define DISPLAY_TASK_STACK_SIZE 2048
#define ACQUISITION_TASK_PRIORITY 2 // Higher priority for data gathering
#define DISPLAY_TASK_PRIORITY 1
#define ACQUISITION_INTERVAL_MS 20 // Read sensor every 20ms
#define DISPLAY_INTERVAL_MS 1000   // Display results every 1 second
#define DISPLAY_TASK_OFFSET_MS 50  // Offset display task start

// --- Signal Conditioning Parameters ---
// Salt & Pepper Filter (Median Filter)
#define MEDIAN_FILTER_WINDOW_SIZE 5 // Must be odd

// Weighted Moving Average Filter
#define WMA_FILTER_WINDOW_SIZE 8
// Example weights (linear decay): newest sample has weight 8, oldest has 1
#define WMA_WEIGHT_SUM 36.0

// Saturation Limits (Example for Voltage)
#define MIN_VOLTAGE 0.0
#define MAX_VOLTAGE REFERENCE_VOLTAGE


class DisplayManager
{
public:
  DisplayManager();
  static void init();
  void updateDisplay(const SensorData &data, const SystemStatus &status);
  static void printData(const ProcessedData_float &data);

private:
  unsigned long lastStdioDisplayTime;
  const unsigned long stdioUpdateInterval = 1000; // Update stdio every 1 second

  // Helper functions
  void displayOnLCD(const SensorData &data, const SystemStatus &status);
  void displayOnSerial(const SensorData &data, const SystemStatus &status);

  // System state to string
  const char *getStateString(SystemState state);
};