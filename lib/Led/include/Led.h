#pragma once

class Led {
public:
    Led(int pin); // Constructor with pin initialization
    ~Led(); // Virtual destructor for proper cleanup

    void on();
    void off();
    void blink(int delayTime);

protected:
    int pin; // Use protected so derived classes can access it
};
