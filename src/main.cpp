#include <Arduino.h>
#include "LedManager.h"
#include "List.h"
#include "io.h"
#include <String.h> 



void setup() {
    Serial.begin(9600);
    while (!Serial) { delay(10); }  

    pinMode(13, OUTPUT);
    IO::init();
}

void loop() {
    char input[128]; 
    printf("Enter a Command: ");

    if (scanf("%127[^\n]", input) != 1) {
        printf("ERROR: Invalid input\n");
    }
    IO::clearInputBuffer();

    IO::trimWhitespace(input);

    if (strcmp(input, "led on") == 0) {
        printf("Turning LED on\n");
        LedManager::on(13);
    } else if (strcmp(input, "led off") == 0) {
        printf("Turning LED off\n");
        LedManager::off(13);
    } else {
        printf("Invalid command: %s\n", input);
    }
    

}