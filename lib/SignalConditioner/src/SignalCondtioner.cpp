
// SignalConditioner.cpp (fixed filename)
#include "SignalConditioner.h"
#include <algorithm>

SignalConditioningModule::SignalConditioningModule() : 
    windowIndex(0), noiseCount(0), totalSamples(0) {
    // Initialize sliding window with zeros
    slidingWindow.fill(0);
    
    // Initialize previous samples
    for (int i = 0; i < WEIGHTS_SIZE; i++) {
        previousSamples[i] = {0, 0, 0.0f, 0};
    }
}

void SignalConditioningModule::init() {
    // Any initialization logic
    Serial.println("Signal conditioning module initialized");
}

ProcessedData SignalConditioningModule::applySaltPepperFilter(const RawSensorData& rawData) {
    ProcessedData data;
    data.rawValue = rawData.analogValue;
    data.timestamp = rawData.timestamp;
    
    // Update sliding window for salt and pepper filter
    slidingWindow[windowIndex] = rawData.analogValue;
    windowIndex = (windowIndex + 1) % WINDOW_SIZE;
    
    // Apply salt and pepper filter
    data.filteredValue = removeSaltPepper(slidingWindow);
    
    // Calculate voltage (assuming 3.3V reference and 12-bit ADC)
    data.voltage = (data.filteredValue / 4095.0f) * 3.3f;
    
    // Track signal quality
    totalSamples++;
    if (isSaltPepper(rawData.analogValue, data.filteredValue, 100)) {
        noiseCount++;
    }
    
    return data;
}

ProcessedData SignalConditioningModule::applyWeightedAverageFilter(const ProcessedData& data) {
    ProcessedData result = data;
    
    // Shift previous samples
    for (int i = WEIGHTS_SIZE - 1; i > 0; i--) {
        previousSamples[i] = previousSamples[i-1];
    }
    previousSamples[0] = data;
    
    // Apply weighted average to filtered value
    float weightedSum = 0.0f;
    float weightSum = 0.0f;
    
    for (int i = 0; i < WEIGHTS_SIZE; i++) {
        if (previousSamples[i].timestamp > 0) {  // Only consider valid samples
            weightedSum += previousSamples[i].filteredValue * weights[i];
            weightSum += weights[i];
        }
    }
    
    // Update filtered value if we have valid previous samples
    if (weightSum > 0) {
        result.filteredValue = static_cast<int>(weightedSum / weightSum);
        // Recalculate voltage with new filtered value
        result.voltage = (result.filteredValue / 4095.0f) * 3.3f;
    }
    
    return result;
}

int SignalConditioningModule::removeSaltPepper(const std::array<int, WINDOW_SIZE>& window) {
    // Create a copy of the window for sorting
    std::array<int, WINDOW_SIZE> sortedWindow = window;
    std::sort(sortedWindow.begin(), sortedWindow.end());
    
    // Return the median value (middle of sorted array)
    return sortedWindow[WINDOW_SIZE / 2];
}

bool SignalConditioningModule::isSaltPepper(int value, int median, int threshold) {
    // If the difference between the value and median exceeds threshold, consider it noise
    return abs(value - median) > threshold;
}

const char* SignalConditioningModule::getSignalQualityDescription() {
    if (totalSamples == 0) {
        return "Unknown";
    }
    
    float noiseRatio = static_cast<float>(noiseCount) / totalSamples;
    
    if (noiseRatio < 0.05) {
        return "Excellent";
    } else if (noiseRatio < 0.10) {
        return "Good";
    } else if (noiseRatio < 0.20) {
        return "Fair";
    } else {
        return "Poor";
    }
}