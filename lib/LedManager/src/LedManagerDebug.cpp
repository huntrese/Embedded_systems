#include "LedManagerDebug.h"
#include "LedManagerBase.h"
#include "Logger.h"
#include "Arduino.h"

int LedManagerDebug::checkPin(int pin) {
    int before = digitalRead(pin);
    Logger::printf("[DEBUG] checkPin(%d) called. Before state: %d", pin, before);
    int state = base.checkPin(pin);
    Logger::printf("[DEBUG] checkPin(%d) returned: %d", pin, state);
    return state;
}

void LedManagerDebug::on(int pin) {
    int before = digitalRead(pin);
    Logger::printf("[DEBUG] on(%d) called. Before state: %d", pin, before);
    base.on(pin);
    int after = digitalRead(pin);
    Logger::printf("[DEBUG] on(%d) completed. After state: %d", pin, after);
}

void LedManagerDebug::off(int pin) {
    int before = digitalRead(pin);
    Logger::printf("[DEBUG] off(%d) called. Before state: %d", pin, before);
    base.off(pin);
    int after = digitalRead(pin);
    Logger::printf("[DEBUG] off(%d) completed. After state: %d", pin, after);
}

void LedManagerDebug::toggle(int pin) {
    int before = digitalRead(pin);
    Logger::printf("[DEBUG] toggle(%d) called. Before state: %d", pin, before);
    base.toggle(pin);
    int after = digitalRead(pin);
    Logger::printf("[DEBUG] toggle(%d) completed. After state: %d", pin, after);
}

void LedManagerDebug::blink(int pin, int times, int onTime, int offTime) {
    Logger::printf("[DEBUG] blink(%d, times=%d, onTime=%d, offTime=%d) called", pin, times, onTime, offTime);
    base.blink(pin, times, onTime, offTime);
    Logger::printf("[DEBUG] blink(%d) completed", pin);
}

void LedManagerDebug::check_group(LinkedList& pins) {
    Logger::printf("[DEBUG] check_group called");
    base.check_group(pins);
}

void LedManagerDebug::on_group(LinkedList& pins) {
    Logger::printf("[DEBUG] on_group called");
    base.on_group(pins);
}

void LedManagerDebug::off_group(LinkedList& pins) {
    Logger::printf("[DEBUG] off_group called");
    base.off_group(pins);
}

void LedManagerDebug::blink_group(LinkedList& pins, int times, int onTime, int offTime) {
    Logger::printf("[DEBUG] blink_group called with times=%d, onTime=%d, offTime=%d", times, onTime, offTime);
    base.blink_group(pins, times, onTime, offTime);
    Logger::printf("[DEBUG] blink_group completed");
}

void LedManagerDebug::blink_group_async(LinkedList& pins, int times, int onTime, int offTime) {
    Logger::printf("[DEBUG] blink_group_async called with times=%d, onTime=%d, offTime=%d", times, onTime, offTime);
    base.blink_group_async(pins, times, onTime, offTime);
    Logger::printf("[DEBUG] blink_group_async completed");
}
