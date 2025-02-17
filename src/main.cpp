#include <Arduino.h>
#include "Led.h"

Led led = Led(13);

void setup() {
    Serial.begin(9600);
    delay(500);
    pinMode(13,OUTPUT);
    
}
        

void loop() {
    led.blink(1000);
    delay(500);


}
