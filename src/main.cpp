#include <Arduino.h>
#include "io.h"
#include "Logger.h"
#include "LedManager.h"
#include "map"
#include "TimerConfigs.h"

#define KEYPAD_PIN 4     // First pin for keypad (needs 8 consecutive pins)
#define LCD_ADDRESS 0x27 // I2C address for LCD
#define LCD_COLS 16      // LCD columns
#define LCD_ROWS 2       // LCD rows
#define GREEN_LED 13     // Success LED
#define RED_LED 12       // Error LED
#define OTHER_LED 2      // Other LED

uint16_t timerMultiple = 0;
std::map<int, std::map<String, int>> dict;
volatile int green_led_state = 0;

// in ms
volatile int taskA_rec = 500;
volatile int taskA_offset = 1000;
volatile int taskA_cnt = taskA_offset;

volatile int taskB_rec = 50;
volatile int taskB_offset = 200;
volatile int taskB_cnt = taskB_offset;

volatile int taskC_rec = 25;
volatile int taskC_offset = 300;
volatile int taskC_cnt = taskC_offset;

volatile int RED_LED_interval = 250;
volatile char c;

void setup()
{

    Serial.begin(9600);

    IO::init(IO::LCD_KEYPAD_MODE);
    IO::initKeypad(KEYPAD_PIN);
    IO::initLCD(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

    setTimerInterval(1, 50, 'm'); // Timer1: 50 milliseconds
    setTimerInterval(2, 1, 'm');  // Timer2: 1 milisecond

    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(OTHER_LED, OUTPUT);

    cli(); // Disable interrupts during setup
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= B00000101;   // Set prescaler to 1024
    TIMSK1 |= B00000010;   // Enable compare match interrupt (OCIE1A)
    OCR1A = TIMER1_PULSES; // Set compare match register A

    TCCR2A = 0;
    TCCR2B = 0;
    TCCR2B |= B00000011;   // Set prescaler to 64
    TIMSK2 |= B00000010;   // Enable compare match interrupt (OCIE2A)
    OCR2A = TIMER2_PULSES; // Set compare match register A

    sei(); // Enable interrupts

    Logger::printf("Timer1 Count: %d", TIMER1_COUNT);
    Logger::printf("Timer2 Count: %d", TIMER2_COUNT);

    LedManager::blink_init(dict, 50);

}

void blink_check_new()
{
    timerMultiple++;
    int elapsedTime = TIMER1_INTERVAL_MS * timerMultiple;

    for (auto &pin_data : dict)
    {
        // .first - key .second - value

        if (elapsedTime % pin_data.second["interval"] == 0)
        {
            LedManager::toggle(pin_data.first);

            if (pin_data.second["times"] > 0)
            {
                pin_data.second["times"] -= 1;
            }

            if (pin_data.second["times"] == 0)
            {
                dict.erase(pin_data.first);
            }
        }
    }

    if (timerMultiple >= TIMER1_COUNT)
    {
        timerMultiple = 0;
    }
}

void loop()
{
    c = (volatile char)getchar();
    IO::clearLCD();

    // Idle time
    printf("A:%d B:%d C:%d\n",taskA_cnt,taskB_cnt,taskC_cnt);
    printf("Green:%d Red:%d\n", green_led_state,RED_LED_interval);
}

void taskA()
{
    if (c == '1')
    {
        LedManager::toggle(GREEN_LED);
        green_led_state = !green_led_state;
        Logger::printf("%d", green_led_state);
        c = '\0';
    }
}
void taskB()
{
    if (!green_led_state)
    {
        LedManager::blink(dict, RED_LED, -1, RED_LED_interval);
    }
    else
    {
        dict.erase(RED_LED);
        LedManager::off(RED_LED);
    }
}
void taskC()
{
    if (c == '2')
    {
        c = '\0';
        if (RED_LED_interval <= 50)
        {
            Logger::printf("Already at min spd.");

            return;
        }
        RED_LED_interval -= 50;
    }
    if (c == '3')
    {
        c = '\0';

        if (RED_LED_interval >= 950)
        {
            Logger::printf("Already at max spd.");
            return;
        }
        RED_LED_interval += 50;
    }
}
ISR(TIMER1_COMPA_vect)
{
    blink_check_new();
    TCNT1 = 0;
}

ISR(TIMER2_COMPA_vect)
{
    if (--taskA_cnt == 0)
    {
        taskA();
        taskA_cnt = taskA_rec;
    }
    if (--taskB_cnt == 0)
    {
        taskB();
        taskB_cnt = taskB_rec;
    }
    if (--taskC_cnt == 0)
    {
        taskC();
        taskC_cnt = taskC_rec;
    }
    TCNT2 = 0;
}
