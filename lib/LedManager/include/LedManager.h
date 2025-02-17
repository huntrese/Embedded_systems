#pragma once
#include "ILedManager.h"

class LedManager {
public:
    static void setDebugMode(bool enabled);
    static int checkPin(int pin);
    static void on(int pin);
    static void off(int pin);
    static void toggle(int pin);
    static void blink(int pin, int times = -1, int onTime = 500, int offTime = 500);
    static void check_group(LinkedList& pins);
    static void on_group(LinkedList& pins);
    static void off_group(LinkedList& pins);
    static void blink_group(LinkedList& pins, int times = -1, int onTime = 500, int offTime = 500);
    static void blink_group_async(LinkedList& pins, int times = -1, int onTime = 500, int offTime = 500);
private:
    static ILedManager* impl;
};
