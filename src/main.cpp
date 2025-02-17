#include <Arduino.h>
#include "LedManager.h"
#include "List.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
// #define LINKEDLIST_INIT(name, ...) LinkedList name{__VA_ARGS__}  outdated -- was kinda cool though

void setup() {
    // LedManager::setDebugMode(true);

    Serial.begin(9600);
    delay(500);

    pinMode(11,OUTPUT);
    pinMode(12,OUTPUT);
    pinMode(13,OUTPUT);
    LedManager::blink(13,2);
    LedManager::blink(12,5);
    delay(1000);
    LedManager::toggle(13);
    
    LinkedList pins = {11, 12, 13};

    LedManager::check_group(pins);
    LedManager::on_group(pins);
    delay(500);
    LedManager::check_group(pins);
    LedManager::off_group(pins);
    delay(500);
    LedManager::check_group(pins);
    LedManager::blink_group(pins,3);
    delay(500);

    LedManager::check_group(pins);
    LedManager::blink_group_async(pins,8,200,40);

}
        

void loop() {


}
