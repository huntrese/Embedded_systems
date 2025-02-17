#pragma once

class Button {
public:
    Button(int pin);
    bool isClicked();
    ~Button();
private:
    int pin;
};