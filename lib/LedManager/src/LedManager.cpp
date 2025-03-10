#include "LedManager.h"

int LedManager::base_timer_interval = 0;


int LedManager::checkPin(int pin) {
    return digitalRead(pin);
}

void LedManager::on(int pin) {
    if (checkPin(pin)) return; // Skip if already on
    digitalWrite(pin, HIGH);
}

void LedManager::off(int pin) {
    if (!checkPin(pin)) return; // Skip if already off
    digitalWrite(pin, LOW);
}

void LedManager::toggle(int pin) {
    digitalWrite(pin, !checkPin(pin));
}

void LedManager::set(int pin, int state) {
    digitalWrite(pin, state);
}

void LedManager::blink(std::map<int, std::map<String, int>>& list, int pin, int times, int delayMs) {
    // Assert that delayMs is a multiple of base_timer_interval
    if (delayMs % base_timer_interval != 0) {
        Serial.print("Error: delayMs (");
        Serial.print(delayMs);
        Serial.print(") must be a multiple of base_timer_interval (");
        Serial.print(base_timer_interval);
        Serial.println(").");
    }



    // Add the blink task to the list
    list[pin]["interval"]=delayMs;
    list[pin]["times"]=2*times;


}

void LedManager::blink_init(std::map<int, std::map<String, int>>& list, int timer_interval) {
    base_timer_interval = timer_interval; // Set the base timer interval

}

