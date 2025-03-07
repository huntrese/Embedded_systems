// #include <Arduino.h>
// #include "List.h"
// #include "io.h"
// #include "Logger.h"
// #include "LedManager.h"
// #include "BlinkList.h"
// #include "KeypadModule.h"

// const uint16_t TIMER_INTERVAL_MS = 50;
// const uint32_t TIMER1_MAX = 65536;
// const uint32_t SYSTEM_CLOCKRATE = 16000000;
// const int PRESCALER = 1024;

// const double TIMER_TICK = static_cast<double>(PRESCALER) / SYSTEM_CLOCKRATE;
// const uint32_t PULSE_COUNTS = round(TIMER_INTERVAL_MS / (TIMER_TICK * 1000));
// const int TIMER_COUNT = floor(TIMER1_MAX / PULSE_COUNTS);

// int timerMultiple = 0;
// volatile BlinkArray blinkList;
// volatile char c;

// void blink_check() {
    
//     timerMultiple++;
//     int elapsedTime = TIMER_INTERVAL_MS * timerMultiple;

//     for (uint16_t i = 0; i < blinkList.size(); i++) {
//         int pin, interval, remainingBlinks;
//         blinkList.get(i, pin, interval, remainingBlinks);

//         if (elapsedTime % interval != 0) {
//             continue;
//         }

//         if (remainingBlinks == 0) {
//             blinkList.remove(i);
//             i--;
//             continue;
//         }

//         if (remainingBlinks > 0) {
//             remainingBlinks--;
//         }

//         LedManager::toggle(pin);
//         blinkList.set(i, pin, interval, remainingBlinks);
//     }

//     if (timerMultiple >= TIMER_COUNT) {
//         timerMultiple = 0;
//     }
// }

// // Hardware configuration
// #define KEYPAD_PIN 4       // First pin for keypad (needs 8 consecutive pins)
// #define LCD_ADDRESS 0x27   // I2C address for LCD
// #define LCD_COLS 16        // LCD columns
// #define LCD_ROWS 2         // LCD rows
// #define GREEN_LED 13       // Success LED
// #define RED_LED 12         // Error LED

// void setup() {
//     IO::init(IO::LCD_KEYPAD_MODE);
//     IO::initKeypad(KEYPAD_PIN);
//     IO::initLCD(LCD_ADDRESS,LCD_COLS,LCD_ROWS);
//     Serial.begin(9600);
//     delay(1000);

//     LedManager::blink_init(blinkList, TIMER_INTERVAL_MS);
//     // LedManager::blink(blinkList, 12, 5, 1000);

//     pinMode(13, OUTPUT);
//     pinMode(12, OUTPUT);
//     pinMode(2, OUTPUT);
//     cli();  // Disable interrupts during setup
//     TCCR1A = 0;  
//     TCCR1B = 0;  

//     TCCR1B |= B00000101;  // Set prescaler to 1024
//     TIMSK1 |= B00000010;  // Enable compare match interrupt (OCIE1A)
//     OCR1A = PULSE_COUNTS; // Set compare match register A

//     sei();  // Enable interrupts

//     Logger::printf("Timer Count: %d", TIMER_COUNT);

// }

// void loop() {
//     c = (volatile char) getchar();
//     if (c=='1'){
//         Logger::printf("\nFirst task");
//     } else if (c=='2'){
//         Logger::printf("\nSecond task");
//     }
//     else if (c=='3'){
//         Logger::printf("\nThird task");
//     }
//     IO::clearLCD();


// }


// void task_1(){
//     LedManager::toggle(13);

// }

// void task_2(){
//     if(!LedManager::checkPin(13)){
//         LedManager::blink(blinkList,12,-1,500);
//     } 
//     // else{
//     //     for (uint16_t i = 0; i < blinkList.size(); i++) {
//     //         int pin, interval, remainingBlinks;
//     //         blinkList.get(i, pin, interval, remainingBlinks);
//     //         if (pin==12){
//     //             blinkList.remove(i);
//     //         }
//     //     }
//     // }
// }


// ISR(TIMER1_COMPA_vect) {
//     if (c=='1'){
//         task_1();
//     } else if (c=='2'){


//     }
//     else if (c=='3'){
//         Logger::printf("\nThird task");
//     }

//     task_2();
//     c='\0';


//     blink_check();
//     TCNT1 = 0;
// }
