#pragma once
#include "Arduino.h"

// Timer 1 Configuration (16-bit timer)
const uint32_t TIMER1_MAX = 65536;
const uint32_t TIMER1_CLOCKRATE = 16000000UL;
const int TIMER1_PRESCALER = 1024;
const double TIMER1_SPEED = (double)TIMER1_CLOCKRATE / TIMER1_PRESCALER;
const double TIMER1_TICK = 1.0 / TIMER1_SPEED;
volatile uint16_t TIMER1_COUNT;
volatile uint32_t TIMER1_PULSES;
uint16_t TIMER1_INTERVAL_MS;

// Timer 2 Configuration (8-bit timer)
const uint32_t TIMER2_MAX = 256;
const uint32_t TIMER2_CLOCKRATE = 8000000UL;
const int TIMER2_PRESCALER = 64;
const double TIMER2_SPEED = (double)TIMER2_CLOCKRATE / TIMER2_PRESCALER;
const double TIMER2_TICK = 1.0 / TIMER2_SPEED;
volatile uint8_t TIMER2_COUNT;
volatile uint32_t TIMER2_PULSES;
uint16_t TIMER2_INTERVAL_MS;

// Allowed interval values in milliseconds
const uint16_t ALLOWED_INTERVALS[] = {1, 5, 10, 50, 100, 500, 1000, 2000, 5000};
const int NUM_INTERVALS = sizeof(ALLOWED_INTERVALS) / sizeof(ALLOWED_INTERVALS[0]);

// Maximum acceptable error percentage
const double MAX_ERROR_PERCENTAGE = 10.0;

// Find the closest allowed interval and calculate error
bool findClosestInterval(double interval_ms, uint16_t& closest_interval, double& error_percentage) {
    closest_interval = ALLOWED_INTERVALS[0];
    double min_diff = abs(interval_ms - closest_interval);
    error_percentage = (min_diff / interval_ms) * 100.0;
    
    for (int i = 1; i < NUM_INTERVALS; i++) {
        double diff = abs(interval_ms - ALLOWED_INTERVALS[i]);
        double current_error = (diff / interval_ms) * 100.0;
        
        if (diff < min_diff) {
            min_diff = diff;
            closest_interval = ALLOWED_INTERVALS[i];
            error_percentage = current_error;
        }
    }
    
    return error_percentage <= MAX_ERROR_PERCENTAGE;
}

// Convert various time units to milliseconds
inline double convertToMilliseconds(double value, char unit) {
    switch(unit) {
        case 's':  // seconds
            return value * 1000.0;
        case 'm':  // milliseconds
            return value;
        case 'u':  // microseconds
            return value / 1000.0;
        case 'n':  // nanoseconds
            return value / 1000000.0;
        default:
            return 0.0;
    }
}

void displayTimerError(int timerNum, double original_interval, double error_percentage) {
    // Ensure Serial is initialized
    if (!Serial) {
        Serial.begin(9600);
    }
    
    Serial.println();    
    Serial.print("Timer ");
    Serial.print(timerNum);
    Serial.println(" ERROR:");
    Serial.print("Requested interval: ");
    Serial.print(original_interval);
    Serial.println(" ms");
    Serial.print("Error exceeds ");
    Serial.print(MAX_ERROR_PERCENTAGE);
    Serial.println("%");
    
    
    // Delay for 2 seconds
    delay(2000);
}

// Set timer interval with various time unit inputs
inline void setTimerInterval(int timerNum, double value, char unit) {
    // Convert to milliseconds
    double interval_ms = convertToMilliseconds(value, unit);
    
    // Find closest interval and check error
    uint16_t rounded_interval;
    double error_percentage;
    
    if (!findClosestInterval(interval_ms, rounded_interval, error_percentage)) {
        // Display error and halt
        displayTimerError(timerNum, interval_ms, error_percentage);
        return;
    }
    
    if (timerNum == 1) {
        // Calculate number of timer ticks needed for the interval
        TIMER1_PULSES = round(rounded_interval / (TIMER1_TICK * 1000.0));
        TIMER1_COUNT = TIMER1_PULSES % TIMER1_MAX;
        TIMER1_INTERVAL_MS = rounded_interval;
    } else if (timerNum == 2) {
        // Calculate number of timer ticks needed for the interval
        TIMER2_PULSES = round(rounded_interval / (TIMER2_TICK * 1000.0));
        TIMER2_COUNT = TIMER2_PULSES % TIMER2_MAX;
        TIMER2_INTERVAL_MS = rounded_interval;
    }
}
