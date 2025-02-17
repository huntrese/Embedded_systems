#pragma once
#include "ILedManager.h"

class LedManagerBase : public ILedManager {
public:
    int checkPin(int pin) override;
    void on(int pin) override;
    void off(int pin) override;
    void toggle(int pin) override;
    void blink(int pin, int times = -1, int onTime = 500, int offTime = 500) override;
    void check_group(LinkedList& pins) override;
    void on_group(LinkedList& pins) override;
    void off_group(LinkedList& pins) override;
    void blink_group(LinkedList& pins, int times = -1, int onTime = 500, int offTime = 500) override;
    void blink_group_async(LinkedList& pins, int times = -1, int onTime = 500, int offTime = 500) override;
};
