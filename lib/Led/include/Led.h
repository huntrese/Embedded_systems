#pragma once

class Led {
public:
    Led(int pin);
    ~Led();

    void on();
    void off();
    void blink(int delayTime);

protected:
    int pin;
};
