#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include <Arduino.h>

class RelayControl {
private:
    uint8_t relayPin;
    bool relayState;

public:
    /**
     * Constructor for RelayControl
     * @param pin GPIO pin connected to relay
     */
    RelayControl(uint8_t pin);
    
    /**
     * Initialize the relay
     */
    void begin();
    
    /**
     * Turn the relay on
     * @return true if operation was successful
     */
    bool turnOn();
    
    /**
     * Turn the relay off
     * @return true if operation was successful
     */
    bool turnOff();
    
    /**
     * Get current relay state
     * @return true if relay is on, false if off
     */
    bool getState();
    
    /**
     * Toggle relay state
     * @return new state of relay
     */
    bool toggle();
};

#endif // RELAY_CONTROL_H