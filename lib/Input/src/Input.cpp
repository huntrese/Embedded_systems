#include "Input.h"

// Keypad configuration
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[KEYPAD_ROWS] = {4, 5, 6, 7}; // Connect to keypad Row0, Row1, Row2, Row3
byte colPins[KEYPAD_COLS] = {8, 9, 10, 11}; // Connect to keypad Col0, Col1, Col2, Col3

InputControl::InputControl(bool useKeypad) {
    serialBufferIndex = 0;
    keyBufferIndex = 0;
    hasKeypad = useKeypad;
    
    if (hasKeypad) {
        keypad = new Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
    }
    
    clearBuffers();
}

void InputControl::begin() {
    // Print welcome message using stdio
    printf("Relay Control System\n");
    printf("Commands: 'relay on', 'relay off'\n");
    
    if (hasKeypad) {
        printf("Keypad: Press A for relay on, B for relay off, # to send\n");
    }
}

uint8_t InputControl::update() {
    uint8_t command = CMD_NONE;
    
    // Check serial input
    while (Serial.available() > 0) {
        char c ='0';
        scanf("%c",&c);
        
        // Process character
        if (c == '\n' || c == '\r') {
            // End of command, process it
            serialBuffer[serialBufferIndex] = '\0';
            command = parseCommand(serialBuffer);
            serialBufferIndex = 0;
            
            // Print confirmation using stdio
            printf("Received command: %s\n", serialBuffer);
            
            clearBuffers();
            return command;
        } else if (serialBufferIndex < sizeof(serialBuffer) - 1) {
            // Add to buffer
            serialBuffer[serialBufferIndex++] = c;
        }
    }
    
    // Check keypad input if available
    if (hasKeypad) {
        char key = keypad->getKey();
        
        if (key) {
            printf("Key pressed: %c\n", key);
            
            // Special keys for predefined commands
            if (key == 'A') {
                strcpy(keyBuffer, "relay on");
                printf("Command: relay on\n");
                return CMD_RELAY_ON;
            } else if (key == 'B') {
                strcpy(keyBuffer, "relay off");
                printf("Command: relay off\n");
                return CMD_RELAY_OFF;
            } else if (key == '#') {
                // Process current buffer
                keyBuffer[keyBufferIndex] = '\0';
                command = parseCommand(keyBuffer);
                keyBufferIndex = 0;
                
                // Print confirmation using stdio
                printf("Keypad command: %s\n", keyBuffer);
                
                clearBuffers();
                return command;
            } else if (keyBufferIndex < sizeof(keyBuffer) - 1) {
                // Add to buffer
                keyBuffer[keyBufferIndex++] = key;
            }
        }
    }
    
    return command;
}

uint8_t InputControl::parseCommand(const char* cmd) {
    // Compare with known commands
    if (strcmp(cmd, "relay on") == 0) {
        return CMD_RELAY_ON;
    } else if (strcmp(cmd, "relay off") == 0) {
        return CMD_RELAY_OFF;
    } else if (strlen(cmd) > 0) {
        return CMD_INVALID;
    }
    
    return CMD_NONE;
}

void InputControl::clearBuffers() {
    memset(serialBuffer, 0, sizeof(serialBuffer));
    memset(keyBuffer, 0, sizeof(keyBuffer));
    serialBufferIndex = 0;
    keyBufferIndex = 0;
}

const char* InputControl::getCommandString(uint8_t cmdCode) {
    switch (cmdCode) {
        case CMD_RELAY_ON:
            return "relay on";
        case CMD_RELAY_OFF:
            return "relay off";
        case CMD_INVALID:
            return "invalid";
        default:
            return "none";
    }
}