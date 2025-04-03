
// DisplayModule.cpp
#include "DisplayModule.h"
// Include IO.h here if it exists in your project
// #include "IO.h"

DisplayModule::DisplayModule() : useLCD(true), lastDisplayUpdate(0) {
}

void DisplayModule::init() {
    // Initialize display hardware if needed
    Serial.println("Display module initialized");
}

void DisplayModule::updateDisplay(const ProcessedData& data, const PhysicalParameter& param) {
    // Update both LCD and Serial displays
    if (useLCD) {
        displayOnLCD(data, param);
    }
    
    // Always update Serial
    displayOnSerial(data, param);
    
    lastDisplayUpdate = millis();
}

void DisplayModule::displayOnLCD(const ProcessedData& data, const PhysicalParameter& param) {
    // If IO class is not available, use direct Serial as placeholder
    // Clear LCD and display data - replace with your actual LCD implementation
    
    // Example using direct Serial output as placeholder
    Serial.println("\n--- LCD Display ---");
    Serial.print("Value: ");
    Serial.print(param.value);
    Serial.print(" ");
    Serial.println(param.unit);
    
    Serial.print("V: ");
    Serial.print(data.voltage);
    Serial.print("V F: ");
    Serial.println(data.filteredValue);
    Serial.println("-----------------");
}

void DisplayModule::displayOnSerial(const ProcessedData& data, const PhysicalParameter& param) {
    // Basic information on Serial
    Serial.print("Value: ");
    Serial.print(param.value);
    Serial.print(" ");
    Serial.print(param.unit);
    Serial.print(" | Voltage: ");
    Serial.print(data.voltage);
    Serial.print(" V | Filtered: ");
    Serial.print(data.filteredValue);
    Serial.print(" | Raw: ");
    Serial.println(data.rawValue);
    
    // Add range indicator
    if (!param.withinRange) {
        Serial.println("⚠️ Value outside valid range!");
    }
}