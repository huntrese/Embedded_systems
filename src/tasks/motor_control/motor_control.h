#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "../../globals/globals.h"

void motorControlTask(void* pvParameters);

#endif
