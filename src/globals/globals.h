#ifndef GLOBALS_H
#define GLOBALS_H

#include "../config/config.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

extern QueueHandle_t motorCommandQueue;

typedef struct {
    int currentPosition;
    int targetPosition;
} SystemStatus;

extern SystemStatus g_systemStatus;

extern SemaphoreHandle_t g_statusMutex;

void initGlobals(void);

#endif