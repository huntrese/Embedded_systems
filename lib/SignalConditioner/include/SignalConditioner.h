// SignalConditioner.h (fixed filename)
#ifndef SIGNAL_CONDITIONER_H
#define SIGNAL_CONDITIONER_H

#include "Arduino.h"
#include <array>
#include "SensorModule.h"  // Include to access RawSensorData struct

// Structure to hold processed data
struct ProcessedData {
    int rawValue;         // Original ADC value
    int filteredValue;    // Filtered value
    float voltage;        // Calculated voltage
    uint32_t timestamp;   // Timestamp of processing
};

class SignalConditioningModule {
public:
    SignalConditioningModule();
    void init();
    ProcessedData applySaltPepperFilter(const RawSensorData& rawData);
    ProcessedData applyWeightedAverageFilter(const ProcessedData& data);
    const char* getSignalQualityDescription();
    
private:
    // Salt and Pepper filter parameters
    static const int WINDOW_SIZE = 5;
    std::array<int, WINDOW_SIZE> slidingWindow;
    int windowIndex;
    
    // Weighted average filter parameters
    static const int WEIGHTS_SIZE = 3;
    const float weights[WEIGHTS_SIZE] = {0.5, 0.3, 0.2};  // Current, previous, and older samples
    std::array<ProcessedData, WEIGHTS_SIZE> previousSamples;
    
    // Signal quality tracking
    int noiseCount;
    int totalSamples;
    
    // Helper functions
    int removeSaltPepper(const std::array<int, WINDOW_SIZE>& window);
    bool isSaltPepper(int value, int median, int threshold);
};

#endif
