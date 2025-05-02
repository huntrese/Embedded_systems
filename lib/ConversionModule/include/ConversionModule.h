// ConversionModule.h
#ifndef CONVERSION_MODULE_H
#define CONVERSION_MODULE_H

#include "Arduino.h"
#include "SignalConditioner.h"  // Changed to match the corrected filename

// Structure to hold physical parameter
struct PhysicalParameter {
    float value;          // Value in physical units
    const char* unit;     // Unit of measurement (e.g., "°C", "lux", "dB")
    bool withinRange;     // Flag to indicate if value is within valid range
};

class ConversionModule {
public:
    ConversionModule();
    void init();
    PhysicalParameter convertToPhysicalParameter(const ProcessedData& data);
    PhysicalParameter applySaturation(const PhysicalParameter& param);
    
private:
    // Conversion parameters
    float voltageDividerRatio;    // For sensors using voltage dividers
    float sensorOffset;           // Offset for linear conversion
    float sensorScale;            // Scale factor for linear conversion
    
    // Range limits for saturation
    float minValidValue;
    float maxValidValue;
};

#endif
