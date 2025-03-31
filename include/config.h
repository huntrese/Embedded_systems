#pragma once

#include "Arduino.h" // Include if needed for types like uint8_t

// --- Pin Definitions ---
constexpr uint8_t LDR_PIN = 34; // ADC1_CH6 on many ESP32 boards (Ensure it's an ADC pin)

// --- ADC Configuration ---
constexpr float ADC_MAX_VALUE = 4095.0;   // For 12-bit ADC (ESP32 default)
constexpr float REFERENCE_VOLTAGE = 3.3; // ESP32 operating voltage

// --- Task Configuration ---
constexpr int ACQUISITION_TASK_STACK_SIZE = 4096; // Increased stack for filtering buffers
constexpr int DISPLAY_TASK_STACK_SIZE = 2048;
constexpr int ACQUISITION_TASK_PRIORITY = 2;      // Higher priority for data gathering
constexpr int DISPLAY_TASK_PRIORITY = 1;
constexpr int ACQUISITION_INTERVAL_MS = 20;      // Read sensor every 20ms
constexpr int DISPLAY_INTERVAL_MS = 1000;        // Display results every 1 second
constexpr int DISPLAY_TASK_OFFSET_MS = 50;       // Offset display task start

// --- Signal Conditioning Parameters ---
// Salt & Pepper Filter (Median Filter)
constexpr size_t MEDIAN_FILTER_WINDOW_SIZE = 5; // Must be odd

// Weighted Moving Average Filter
constexpr size_t WMA_FILTER_WINDOW_SIZE = 8;
// Example weights (linear decay): newest sample has weight 8, oldest has 1
const float WMA_WEIGHTS[WMA_FILTER_WINDOW_SIZE] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
// Sum of weights for normalization (1+2+...+8 = 36)
constexpr float WMA_WEIGHT_SUM = 36.0;

// Saturation Limits (Example for Voltage)
constexpr float MIN_VOLTAGE = 0.0;
constexpr float MAX_VOLTAGE = REFERENCE_VOLTAGE;

// --- Data Structure for Sharing ---
struct ProcessedData {
    int rawADC = 0;
    float medianFilteredADC = 0.0;
    float wmaFilteredADC = 0.0;
    float voltage = 0.0;
    // Add physical parameter if conversion formula is known/calibrated
    // float lightLevel = 0.0;
};