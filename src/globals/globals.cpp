#include "globals.h"

QueueHandle_t motorCommandQueue;
SystemStatus g_systemStatus;
SemaphoreHandle_t g_statusMutex;

void initGlobals(void)
{
    motorCommandQueue = xQueueCreate(5, sizeof(MotorCommand));
    g_statusMutex = xSemaphoreCreateMutex();

    g_systemStatus.currentPosition = 0;
    g_systemStatus.targetPosition = 0;
}