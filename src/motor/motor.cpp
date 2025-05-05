#include "motor.h"
#include "../globals/globals.h"

static int currentPosition = 0;
static int targetSetpoint = DEFAULT_SETPOINT;
static int hysteresisBand = DEFAULT_HYSTERESIS;

void initMotor()
{
    pinMode(MOTOR_IN1_PIN, OUTPUT);
    pinMode(MOTOR_IN2_PIN, OUTPUT);
    pinMode(MOTOR_ENA_PIN, OUTPUT);

    digitalWrite(MOTOR_IN1_PIN, LOW);
    digitalWrite(MOTOR_IN2_PIN, LOW);
    analogWrite(MOTOR_ENA_PIN, 0);

    currentPosition = 0;
    targetSetpoint = DEFAULT_SETPOINT;
    hysteresisBand = DEFAULT_HYSTERESIS;
}

void motorUpdatePosition(int position)
{
    currentPosition = position;
    int lowerBound = targetSetpoint - hysteresisBand;
    int upperBound = targetSetpoint + hysteresisBand;

    uint8_t pwmValue = map(MOTOR_POWER, 0, 100, 0, 255);

    if (currentPosition <= lowerBound) {
        digitalWrite(MOTOR_IN1_PIN, LOW);
        digitalWrite(MOTOR_IN2_PIN, LOW);
        analogWrite(MOTOR_ENA_PIN, 0);
    } else if (currentPosition >= upperBound) {
        digitalWrite(MOTOR_IN1_PIN, LOW);
        digitalWrite(MOTOR_IN2_PIN, HIGH);
        analogWrite(MOTOR_ENA_PIN, pwmValue);
    }
}

void motorSetSetpoint(int setpoint)
{
    targetSetpoint = setpoint;
    motorUpdatePosition(currentPosition);
}

int motorGetSetpoint()
{
    return targetSetpoint;
}