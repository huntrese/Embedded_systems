#include "Clickable.h"
#include <Arduino.h>

bool isClicked(int pin) {
    return digitalRead(pin);
}