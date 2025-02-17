#include "LedManager.h"
#include "LedManagerBase.h"
#include "LedManagerDebug.h"
#include "Logger.h"

// Initialize with the base (non‑debug) implementation.
ILedManager* LedManager::impl = new LedManagerBase();

int LedManager::checkPin(int pin) {
    return impl->checkPin(pin);
}

void LedManager::on(int pin) {
    impl->on(pin);
}

void LedManager::off(int pin) {
    impl->off(pin);
}

void LedManager::toggle(int pin) {
    impl->toggle(pin);
}

void LedManager::blink(int pin, int times, int onTime, int offTime) {
    impl->blink(pin, times, onTime, offTime);
}

void LedManager::check_group(LinkedList& pins) {
    impl->check_group(pins);
}

void LedManager::on_group(LinkedList& pins) {
    impl->on_group(pins);
}

void LedManager::off_group(LinkedList& pins) {
    impl->off_group(pins);
}

void LedManager::blink_group(LinkedList& pins, int times, int onTime, int offTime) {
    impl->blink_group(pins, times, onTime, offTime);
}

void LedManager::blink_group_async(LinkedList& pins, int times, int onTime, int offTime) {
    impl->blink_group_async(pins, times, onTime, offTime);
}

void LedManager::setDebugMode(bool enabled) {
    delete impl;  // Clean up the previous implementation
    if (enabled) {
        impl = new LedManagerDebug();
        Logger::printf("[DEBUG] Debug mode enabled.");
    } else {
        impl = new LedManagerBase();
        Logger::printf("Debug mode disabled.");
    }
}
