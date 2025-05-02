// DisplayModule.h
#ifndef DISPLAY_MODULE_H
#define DISPLAY_MODULE_H

#include "Arduino.h"
#include "SignalConditioner.h"  // Changed to match the corrected filename
#include "ConversionModule.h"

class DisplayModule {
public:
    DisplayModule();
    void init();
    void updateDisplay(const ProcessedData& data, const PhysicalParameter& param);
    
private:
    bool useLCD;              // Flag to indicate if LCD display should be used
    unsigned long lastDisplayUpdate;
    
    // Helper methods for different display formats
    void displayOnLCD(const ProcessedData& data, const PhysicalParameter& param);
    void displayOnSerial(const ProcessedData& data, const PhysicalParameter& param);
};

#endif
