#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../../config/config.h"
#include "../../globals/globals.h"
#include "../../motor/motor.h"
#include <Arduino.h>
#include <stdio.h>

void potentiometerTask(void* pvParameters);

#endif
