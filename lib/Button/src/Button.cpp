#include "Button.h"
#include <Arduino.h>

Button::Button(int pin) : pin(pin) {}

Button::~Button() {}

bool Button::isClicked() {
    return !digitalRead(pin);
}