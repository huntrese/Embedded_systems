#include <Arduino.h>
#include "LedManager.h"
#include "List.h"
#include "io.h"
#include "String.h"


void setup() {

    Serial.begin(9600);
    delay(500);

    // pinMode(11,OUTPUT);
    // pinMode(12,OUTPUT);
    pinMode(13,OUTPUT);
    // LedManager::blink(13,2);
    // LedManager::blink(12,5);
    // delay(1000);
    // LedManager::toggle(13);
    
    // LinkedList pins = {11, 12, 13};

    // LedManager::check_group(pins);
    // LedManager::on_group(pins);
    // delay(500);
    // LedManager::check_group(pins);
    // LedManager::off_group(pins);
    // delay(500);
    // LedManager::check_group(pins);
    // LedManager::blink_group(pins,3);
    // delay(500);

    // LedManager::check_group(pins);
    // LedManager::blink_group_async(pins,8,200,40);
    IO::init();

}
        

void loop() {
    char input[128];
    printf("Enter a Command: "); 
    
    if (scanf("%127s", input) != 1) { 
        printf("\nERROR: Invalid input\n");
        IO::clearInputBuffer(); // Clear bad input from buffer
        return;
    }

    if (strcmp(input,"gelu")==0){
        printf("\ngood shit\n");
        LedManager::on(13);
    } else if (strcmp(input,"geluu")==0){
        printf("\namazing shit\n");
        LedManager::off(13);

    } else {
        printf("\nInvalid input\n");
    }
    


}
