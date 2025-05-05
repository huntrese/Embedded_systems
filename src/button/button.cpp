#include "button.h"
#include "../config/config.h"

#define DEBOUNCE_DELAY_MS 50

static bool lastIncButtonState = HIGH;
static bool lastDecButtonState = HIGH;

void initButtons()
{
    pinMode(BUTTON_INC_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DEC_PIN, INPUT_PULLUP);
}

bool buttonCheckIncrement()
{
    bool pressed = false;
    bool currentIncState = digitalRead(BUTTON_INC_PIN);

    if (currentIncState == LOW && lastIncButtonState == HIGH) {
        pressed = true;
    }

    lastIncButtonState = currentIncState;
    return pressed;
}

bool buttonCheckDecrement()
{
    bool pressed = false;
    bool currentDecState = digitalRead(BUTTON_DEC_PIN);

    if (currentDecState == LOW && lastDecButtonState == HIGH) {
        pressed = true;
    }

    lastDecButtonState = currentDecState;
    return pressed;
}