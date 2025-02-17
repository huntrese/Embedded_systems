#include <Arduino.h>
#include "Led.h"
#include "Button.h"

Led led = Led(13);
Button button = Button(12);

void setup() {
    Serial.begin(9600);
    delay(500);
    pinMode(13,OUTPUT);
    pinMode(12,INPUT_PULLUP);
}
        

void loop() {
    if (button.isClicked()) {
        led.on();
    } else {
        led.off();
    }
}
