#include "RelayControl.h"

RelayControl::RelayControl(uint8_t pin) {
    relayPin = pin;
    relayState = false; // Initialize as OFF
}

void RelayControl::begin() {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW); // Most relay modules are active LOW
    relayState = false;
}

bool RelayControl::turnOn() {
    digitalWrite(relayPin, HIGH);
    relayState = true;
    return true;
}

bool RelayControl::turnOff() {
    digitalWrite(relayPin, LOW);
    relayState = false;
    return true;
}

bool RelayControl::getState() {
    return relayState;
}

bool RelayControl::toggle() {
    if (relayState) {
        return turnOff();
    } else {
        return turnOn();
    }
}