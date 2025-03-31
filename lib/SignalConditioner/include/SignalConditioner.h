#pragma once

#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include "config.h"

class SignalConditioner {
public:
    SignalConditioner();
    void init();
    ProcessedData processNewRawValue(int rawValue); // Applies all conditioning steps

private:
    // Median Filter (Salt & Pepper)
    std::vector<int> medianBuffer;
    size_t medianBufferIndex;
    int applyMedianFilter(int newValue);

    // Weighted Moving Average Filter
    std::vector<float> wmaBuffer;
    size_t wmaBufferIndex;
    float applyWMAFilter(float newValue);

    // Conversion & Saturation
    float convertADCToVoltage(float adcValue);
    float saturateValue(float value, float minVal, float maxVal);

    bool medianBufferFilled;
    bool wmaBufferFilled;
};