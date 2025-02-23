#pragma once
#include "BlinkList.h"
#include <assert.h>

class LedManager {
public:
    // Static methods for LED control
    static int checkPin(int pin);
    static void on(int pin);
    static void off(int pin);
    static void toggle(int pin);
    static void blink(volatile BlinkArray& list, int pin, int times = -1, int delayMs = 500);
    static void blink_init(volatile BlinkArray& list, int timer_interval);

private:
    static int base_timer_interval; // Stores the base timer interval for blink operations
};