#include <Arduino.h>
#include "Led.h"
#include "Button.h"
#include "io.h"
#include <stdio.h>
#include <Arduino.h>

Led led = Led(13);
Button button = Button(12);

void setup() {
    Serial.begin(9600);
    delay(500);
    pinMode(13,OUTPUT);
    pinMode(12,INPUT_PULLUP);
    io::init();
}

void loop() {
    printf("Enter your command: ");
    char* input = io::input();

    if (strcmp(input, "led on") == 0) {
        led.on();
    } else if (strcmp(input, "led off") == 0) {
        led.off();
    }
    printf("\n");
    // int nr;
    // scanf("%d",nr);
    // printf("%d",nr);
}
