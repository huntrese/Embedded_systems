#ifndef CONFIG_H
#define CONFIG_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define MOTOR_IN1_PIN 25
#define MOTOR_IN2_PIN 26
#define MOTOR_ENA_PIN 27

#define POT_PIN 34

#define BUTTON_INC_PIN 32
#define BUTTON_DEC_PIN 33

#define LED_PIN 2

#define POTENTIOMETER_TASK_PRIORITY 2
#define MOTOR_TASK_PRIORITY 3
#define BUTTON_TASK_PRIORITY 2
#define STATUS_REPORTER_TASK_PRIORITY 1

#define MOTOR_POWER 50
#define DEFAULT_SETPOINT 0
#define DEFAULT_HYSTERESIS 5
#define BUTTON_SETPOINT_CHANGE_UNITS 5

#define SERIAL_BAUD_RATE 115200

typedef enum {
    MOTOR_UPDATE_POSITION,
    MOTOR_SET_SETPOINT
} MotorCommandType;

typedef struct {
    MotorCommandType type;
    int value;
} MotorCommand;

#endif
