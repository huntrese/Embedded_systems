#pragma once
#include "List.h"   
#include "Arduino.h"

class ILedManager {
public:
    virtual int checkPin(int pin) = 0;
    virtual void on(int pin) = 0;
    virtual void off(int pin) = 0;
    virtual void toggle(int pin) = 0;
    virtual void blink(int pin, int times = -1, int onTime = 500, int offTime = 500) = 0;
    virtual void check_group(LinkedList& pins) = 0;
    virtual void on_group(LinkedList& pins) = 0;
    virtual void off_group(LinkedList& pins) = 0;
    virtual void blink_group(LinkedList& pins, int times = -1, int onTime = 500, int offTime = 500) = 0;
    virtual void blink_group_async(LinkedList& pins, int times = -1, int onTime = 500, int offTime = 500) = 0;
    virtual ~ILedManager() {}
};
