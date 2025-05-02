
// ConversionModule.cpp
#include "ConversionModule.h"

ConversionModule::ConversionModule() :
    voltageDividerRatio(1.0f),  // Default is 1.0 (no divider)
    sensorOffset(0.0f),         // Default offset is 0
    sensorScale(1.0f),          // Default scale is 1
    minValidValue(0.0f),        // Default minimum valid value
    maxValidValue(100.0f)       // Default maximum valid value
{
    // These values should be adjusted based on the specific sensor used
}

void ConversionModule::init() {
    // Initialize sensor-specific conversion parameters
    
    // Example for a temperature sensor (e.g., LM35)
    // sensorScale = 100.0f;  // LM35 outputs 10mV per degree Celsius
    // minValidValue = -55.0f;
    // maxValidValue = 150.0f;
    
    Serial.println("Conversion module initialized");
}

PhysicalParameter ConversionModule::convertToPhysicalParameter(const ProcessedData& data) {
    PhysicalParameter param;
    
    // Apply voltage divider correction if needed
    float correctedVoltage = data.voltage * voltageDividerRatio;
    
    // Convert voltage to physical parameter using linear formula
    param.value = (correctedVoltage - sensorOffset) * sensorScale;
    
    // Set the unit based on sensor type
    param.unit = "units";  // Replace with actual unit (e.g., "°C", "lux", etc.)
    
    // Check if within valid range
    param.withinRange = (param.value >= minValidValue && param.value <= maxValidValue);
    
    return param;
}

PhysicalParameter ConversionModule::applySaturation(const PhysicalParameter& param) {
    PhysicalParameter saturatedParam = param;
    
    // Apply saturation to ensure value is within valid range
    if (param.value < minValidValue) {
        saturatedParam.value = minValidValue;
        saturatedParam.withinRange = false;
    } else if (param.value > maxValidValue) {
        saturatedParam.value = maxValidValue;
        saturatedParam.withinRange = false;
    }
    
    return saturatedParam;
}