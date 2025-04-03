#include "SignalConditioner.h"
#include "Arduino.h" // For Serial.print debugging if needed

SignalConditioner::SignalConditioner() :
    medianBufferIndex(0),
    wmaBufferIndex(0),
    medianBufferFilled(false),
    wmaBufferFilled(false)
{}

void SignalConditioner::init() {
    // Initialize buffers with 0 or a sensible default
    medianBuffer.resize(MEDIAN_FILTER_WINDOW_SIZE, 0);
    wmaBuffer.resize(WMA_FILTER_WINDOW_SIZE, 0.0f);
    Serial.println("SignalConditioner initialized.");
}

ProcessedData_float SignalConditioner::processNewRawValue(int rawValue) {
    ProcessedData_float result;
    result.rawADC = rawValue;

    // 1. Apply Median Filter (Salt & Pepper removal)
    result.medianFilteredADC = static_cast<float>(applyMedianFilter(rawValue));

    // 2. Apply Weighted Moving Average Filter (Smoothing)
    result.wmaFilteredADC = applyWMAFilter(result.medianFilteredADC);

    // 3. Convert ADC (filtered) to Voltage
    result.voltage = convertADCToVoltage(result.wmaFilteredADC);

    // 4. Apply Saturation to Voltage
    result.voltage = saturateValue(result.voltage, MIN_VOLTAGE, MAX_VOLTAGE);

    // 5. Convert Voltage to Physical Parameter (if applicable)
    // Example: Placeholder for Lux - requires calibration/datasheet formula
    // result.lightLevel = calculateLightLevel(result.voltage);
    // result.lightLevel = saturateValue(result.lightLevel, MIN_LUX, MAX_LUX);

    return result;
}

// --- Private Filter Methods ---

int SignalConditioner::applyMedianFilter(int newValue) {
    medianBuffer[medianBufferIndex] = newValue;
    medianBufferIndex = (medianBufferIndex + 1) % MEDIAN_FILTER_WINDOW_SIZE;

    // Check if buffer is filled for the first time
    if (!medianBufferFilled && medianBufferIndex == 0) {
        medianBufferFilled = true;
    }

    // Until the buffer is full, return the current value or a simple average?
    // Let's return the current value until buffer is full for simplicity.
    if (!medianBufferFilled) {
        return newValue;
    }

    // Create a temporary copy and sort it to find the median
    std::vector<int> sortedBuffer = medianBuffer;
    std::sort(sortedBuffer.begin(), sortedBuffer.end());

    // Return the middle element
    return sortedBuffer[MEDIAN_FILTER_WINDOW_SIZE / 2];
}

float SignalConditioner::applyWMAFilter(float newValue) {
    wmaBuffer[wmaBufferIndex] = newValue;
    wmaBufferIndex = (wmaBufferIndex + 1) % WMA_FILTER_WINDOW_SIZE;

    if (!wmaBufferFilled && wmaBufferIndex == 0) {
        wmaBufferFilled = true;
    }

    if (!wmaBufferFilled) {
        // Calculate average of filled part until buffer is full
        float currentSum = 0;
        for(size_t i = 0; i < wmaBufferIndex; ++i) {
            currentSum += wmaBuffer[i];
        }
        return (wmaBufferIndex > 0) ? (currentSum / wmaBufferIndex) : newValue;
    }

    // Apply WMA calculation
    float weightedSum = 0;
    float currentWeightSum = 0; // In case weights don't sum to 1 easily
    for (size_t i = 0; i < WMA_FILTER_WINDOW_SIZE; ++i) {
        // Access buffer circularly, starting from the oldest element relative to current index
        size_t bufferIdx = (wmaBufferIndex + i) % WMA_FILTER_WINDOW_SIZE;
        weightedSum += wmaBuffer[bufferIdx] * WMA_WEIGHTS[i];
        // currentWeightSum += WMA_WEIGHTS[i]; // This should equal WMA_WEIGHT_SUM if weights are static
    }

    return weightedSum / WMA_WEIGHT_SUM;
}

// --- Private Conversion & Saturation Methods ---

float SignalConditioner::convertADCToVoltage(float adcValue) {
    // Ensure float division
    return (adcValue / ADC_MAX_VALUE) * REFERENCE_VOLTAGE;
}

float SignalConditioner::saturateValue(float value, float minVal, float maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
    // Or using std::max and std::min:
    // return std::max(minVal, std::min(value, maxVal));
}