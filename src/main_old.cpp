// #include <Arduino.h>
// #include "List.h"
// #include "io.h"
// #include "Logger.h"
// #include "LedManager.h"
// #include "BlinkList.h"
// #include "KeypadModule.h"
// #include <map>
// #include "TimerConfigs.h"

// #define KEYPAD_PIN 4       // First pin for keypad (needs 8 consecutive pins)
// #define LCD_ADDRESS 0x27   // I2C address for LCD
// #define LCD_COLS 16        // LCD columns
// #define LCD_ROWS 2         // LCD rows
// #define GREEN_LED 13       // Success LED
// #define RED_LED 12         // Error LED
// #define OTHER_LED 2        // Other LED

// uint16_t timerMultiple = 0;
// std::map<int, std::map<String, int>> dict;
// volatile int green_led_state = 0;
// volatile char c;

// void setup() {
//     Serial.begin(9600);
//     IO::init(IO::LCD_KEYPAD_MODE);
//     IO::initKeypad(KEYPAD_PIN);
//     IO::initLCD(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

//     setTimerInterval(1, 50, 'm'); // Timer1: 50 milliseconds
//     setTimerInterval(2, 1, 'm');  // Timer2: 1 millisecond

//     pinMode(GREEN_LED, OUTPUT);
//     pinMode(RED_LED, OUTPUT);
//     pinMode(OTHER_LED, OUTPUT);

//     cli();  // Disable interrupts during setup
//     TCCR1A = 0;  
//     TCCR1B = 0;  
//     TCCR1B |= B00000101;  // Set prescaler to 1024
//     TIMSK1 |= B00000010;  // Enable compare match interrupt (OCIE1A)
//     OCR1A = TIMER1_PULSES;

//     TCCR2A = 0;
//     TCCR2B = 0;
//     TCCR2B |= B00000011;  // Set prescaler to 64
//     TIMSK2 |= B00000010;  // Enable compare match interrupt (OCIE2A)
//     OCR2A = TIMER2_PULSES;

//     sei();  // Enable interrupts

//     Logger::printf("Timer1 Count: %d", TIMER1_COUNT);
//     Logger::printf("Timer2 Count: %d", TIMER2_COUNT);

//     LedManager::blink_init(dict, 50);
//     // LedManager::blink(dict, OTHER_LED, -1, 500);
//     // LedManager::blink(dict, RED_LED, 7, 1000);
//     // LedManager::blink(dict, GREEN_LED, -1, 100);
// }

// void blink_check_new() {
//     timerMultiple++;
//     int elapsedTime = TIMER1_INTERVAL_MS * timerMultiple;

//     for (auto it = dict.begin(); it != dict.end();) {
//         if (elapsedTime % it->second["interval"] == 0) {
//             LedManager::toggle(it->first);
//             if (it->second["times"] > 0) {
//                 it->second["times"]--;
//             }
//             if (it->second["times"] == 0) {
//                 dict.erase(it);
//                 continue;
//             }
//         }
//         ++it;
//     }

//     if (timerMultiple >= TIMER1_COUNT) {
//         timerMultiple = 0;
//     }
// }

// void loop() {
//     c = (volatile char)getchar();
//     if (c == '1') {
//         Logger::printf("\nFirst task");
//     } else if (c == '2') {
//         Logger::printf("\nSecond task");
//     } else if (c == '3') {
//         Logger::printf("\nThird task");
//     }
//     IO::clearLCD();
// }

// void taskA() {
//     digitalWrite(GREEN_LED, digitalRead(GREEN_LED) ^ 1);
//     green_led_state = !green_led_state;
// }

// void taskB() {
//     if (!green_led_state) {
//         LedManager::blink(dict, RED_LED, -1, 250);
//     } else {
//         dict.erase(GREEN_LED);
//     }
// }

// void taskC() {
//     Logger::printf("\nExecuting Task C");
// }

// ISR(TIMER1_COMPA_vect) {
//     if (c == '1') {
//         taskA();
//     } else if (c == '2') {
//         taskB();
//     } else if (c == '3') {
//         taskC();
//     }
//     c = '\0';
//     blink_check_new();
//     TCNT1 = 0;
// }

// ISR(TIMER2_COMPA_vect) {
//     TCNT2 = 0;
// }