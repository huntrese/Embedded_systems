#ifndef INPUT_CONTROL_H
#define INPUT_CONTROL_H

#include <Arduino.h>
#include <Keypad.h>
#include <stdio.h>

// Command constants
#define CMD_NONE        0
#define CMD_RELAY_ON    1
#define CMD_RELAY_OFF   2
#define CMD_INVALID     3

class InputControl {
private:
    // For serial input
    char serialBuffer[32];
    uint8_t serialBufferIndex;
    
    // For keypad input
    Keypad* keypad;
    bool hasKeypad;
    char keyBuffer[32];
    uint8_t keyBufferIndex;
    
    // Command processing
    uint8_t parseCommand(const char* cmd);
    void clearBuffers();

public:
    /**
     * Constructor for InputControl
     * @param useKeypad Whether to initialize keypad
     */
    InputControl(bool useKeypad = false);
    
    /**
     * Initialize the input devices
     */
    void begin();
    
    /**
     * Update input states and check for commands
     * @return Command code (CMD_NONE, CMD_RELAY_ON, CMD_RELAY_OFF, CMD_INVALID)
     */
    uint8_t update();
    
    /**
     * Get command string representation
     * @param cmdCode Command code
     * @return String representation of command
     */
    const char* getCommandString(uint8_t cmdCode);
};

#endif // INPUT_CONTROL_H