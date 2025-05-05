#ifndef STATUS_REPORTER_H
#define STATUS_REPORTER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

void statusReporterTask(void* pvParameters);

#endif