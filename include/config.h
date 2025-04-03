#pragma once


// --- Pin Definitions ---
#define LDR_PIN 34 // ADC1_CH6 on many ESP32 boards (Ensure it's an ADC pin)

// --- ADC Configuration ---
#define ADC_MAX_VALUE 4095.0   // For 12-bit ADC (ESP32 default)
#define REFERENCE_VOLTAGE 3.3 // ESP32 operating voltage

// --- Task Configuration ---
#define ACQUISITION_TASK_STACK_SIZE 4096 // Increased stack for filtering buffers
#define DISPLAY_TASK_STACK_SIZE 2048
#define ACQUISITION_TASK_PRIORITY 2      // Higher priority for data gathering
#define DISPLAY_TASK_PRIORITY 1
#define ACQUISITION_INTERVAL_MS 20      // Read sensor every 20ms
#define DISPLAY_INTERVAL_MS 1000        // Display results every 1 second
#define DISPLAY_TASK_OFFSET_MS 50       // Offset display task start

// --- Signal Conditioning Parameters ---
// Salt & Pepper Filter (Median Filter)
#define MEDIAN_FILTER_WINDOW_SIZE 5 // Must be odd

// Weighted Moving Average Filter
#define WMA_FILTER_WINDOW_SIZE 8
// Example weights (linear decay): newest sample has weight 8, oldest has 1
// Sum of weights for normalization (1+2+...+8 = 36)
#define WMA_WEIGHT_SUM 36.0

// Saturation Limits (Example for Voltage)
#define MIN_VOLTAGE 0.0
#define MAX_VOLTAGE REFERENCE_VOLTAGE


#ifndef PROCESSED_DATA_H
#define PROCESSED_DATA_H

// --- Templated Data Structure for Sharing ---
#define DECLARE_PROCESSED_DATA(T)                                                           \
  struct ProcessedData_##T                                                                  \
  {                                                                                         \
    T rawADC;                                                                               \
    T medianFilteredADC;                                                                    \
    T wmaFilteredADC;                                                                       \
    T voltage;                                                                              \
    ProcessedData_##T() : rawADC(0), medianFilteredADC(0), wmaFilteredADC(0), voltage(0) {} \
  }

// Instantiate common types
DECLARE_PROCESSED_DATA(int);
DECLARE_PROCESSED_DATA(float);

#endif // PROCESSED_DATA_H
