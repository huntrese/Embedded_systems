#pragma once

class Button {
public:
    bool virtual isClicked() = 0;
    ~Button();
};