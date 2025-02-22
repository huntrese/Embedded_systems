#include <Arduino.h>
#include "List.h"
#include "io.h"
#include "Logger.h"

const int timer_interval = 50; // ms
const int TIMER1_MAX = 65536;
const int SYSTEM_CLOCKRATE = 16000000;
const int PRESCALER = 1024;
const int TIMER_TICK = SYSTEM_CLOCKRATE/PRESCALER;
const int TIMER_COUNT = round(timer_interval*TIMER_TICK);


bool LED_STATE = true;
int timerMultiple = 0;
int list[] = {13,12};
int delays[] = {250,1000};

// Hardware configuration
#define KEYPAD_PIN 4       // First pin for keypad (needs 8 consecutive pins)
#define LCD_ADDRESS 0x27   // I2C address for LCD
#define LCD_COLS 16        // LCD columns
#define LCD_ROWS 2         // LCD rows
#define GREEN_LED 13       // Success LED
#define RED_LED 12         // Error LED

void setup() {
    pinMode(13, OUTPUT);        //Set the pin to be OUTPUT
    pinMode(12, OUTPUT);        //Set the pin to be OUTPUT
    cli();                      //stop interrupts for till we make the settings
                                /*1. First we reset the control register to make sure we start with everything disabled.*/
    TCCR1A = 0;                 // Reset entire TCCR1A to 0 
    TCCR1B = 0;                 // Reset entire TCCR1B to 0

                                /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
    TCCR1B |= B00000101;        //Set CS12 to 1 so we get prescalar 256  

                                /*3. We enable compare match mode on register A*/
    TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 

                                /*4. Set the value of register A to 31250*/
    OCR1A = TIMER_COUNT;             //Finally we set compare register A to this value 

    sei();                     //Enable back the interrupts

    IO::init();
    Serial.begin(9600);
    delay(1000);
    // list.addPin(40);

    // list.forEach([](int pin){Logger::printf("%d",pin);});
}

void loop() {
    // put your main code here, to run repeatedly:
}

//With the settings above, this IRS will trigger each 50ms.
ISR(TIMER1_COMPA_vect){
    timerMultiple+=1;
    int thisMultiple=50*timerMultiple;
    for (int i=0;i<sizeof(list)/sizeof(int);i++){
        Logger::printf("%d %d",i, thisMultiple);
        if(thisMultiple%delays[i]==0){
            digitalWrite(list[i],!digitalRead(list[i]));
            // Serial.println(i);
            Logger::printf("toggled %d",list[i]);

        }
    }    
    TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
    if (timerMultiple>=84){
        timerMultiple=0;
    }

}