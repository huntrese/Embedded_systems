#ifndef MOTOR_H
#define MOTOR_H

#include "../config/config.h"
#include <Arduino.h>

void initMotor();
void motorUpdatePosition(int position);
void motorSetSetpoint(int setpoint);
int motorGetSetpoint();

#endif