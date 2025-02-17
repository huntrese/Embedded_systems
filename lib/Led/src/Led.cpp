#include "Led.h"
#include <Arduino.h>

Led::Led(int pin) : pin(pin) {}

Led::~Led() {}

void Led::on() {
    return digitalWrite(pin, HIGH);
}

void Led::off() {
    return digitalWrite(pin, LOW);
}

void Led::blink(int delayTime) {
    on();
    delay(delayTime);
    off();
}