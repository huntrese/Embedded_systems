#include "led.h"
#include "../config/config.h"

void initLED()
{
    pinMode(LED_PIN, OUTPUT);
}

void setLED(bool state)
{
    digitalWrite(LED_PIN, state ? HIGH : LOW);
}