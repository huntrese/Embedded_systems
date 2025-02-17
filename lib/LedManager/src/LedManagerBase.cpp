#include "LedManagerBase.h"
#include "Arduino.h"

int LedManagerBase::checkPin(int pin) {
    return digitalRead(pin);
}

void LedManagerBase::on(int pin) {
    if (checkPin(pin) == 1) return;
    digitalWrite(pin, HIGH);
}

void LedManagerBase::off(int pin) {
    if (checkPin(pin) == 0) return;
    digitalWrite(pin, LOW);
}

void LedManagerBase::toggle(int pin) {
    digitalWrite(pin, !checkPin(pin));
}

void LedManagerBase::blink(int pin, int times, int onTime, int offTime) {
    if (times == -1) {
        while (true) {
            on(pin);
            delay(onTime);
            off(pin);
            delay(offTime);
        }
    } else {
        for (int i = 0; i < times; ++i) {
            on(pin);
            delay(onTime);
            off(pin);
            delay(offTime);
        }
    }
}

void LedManagerBase::check_group(LinkedList& pins) {
    pins.forEach([this](int pin) { checkPin(pin); });
}

void LedManagerBase::on_group(LinkedList& pins) {
    pins.forEach([this](int pin) { on(pin); });
}

void LedManagerBase::off_group(LinkedList& pins) {
    pins.forEach([this](int pin) { off(pin); });
}

void LedManagerBase::blink_group(LinkedList& pins, int times, int onTime, int offTime) {
    if (times == -1) {
        while (true) {
            pins.forEach([this](int pin) { on(pin); });
            delay(onTime);
            pins.forEach([this](int pin) { off(pin); });
            delay(offTime);
        }
    } else {
        for (int i = 0; i < times; ++i) {
            pins.forEach([this](int pin) { on(pin); });
            delay(onTime);
            pins.forEach([this](int pin) { off(pin); });
            delay(offTime);
        }
    }
}

void LedManagerBase::blink_group_async(LinkedList& pins, int times, int onTime, int offTime) {
    if (times == -1) {
        while (true) {
            pins.forEach([this, onTime, offTime](int pin) { blink(pin, 1, onTime, offTime); });
        }
    } else {
        for (int i = 0; i < times; ++i) {
            pins.forEach([this, onTime, offTime](int pin) { blink(pin, 1, onTime, offTime); });
        }
    }
}
