#include "Arduino.h"
#include "LedManager.h"
#include "io.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#define BUTTON_PIN 12  // Button on GPIO 12
#define LED_PIN 13    // LED on GPIO 13

// Global variables for synchronization and data
SemaphoreHandle_t xButtonSemaphore;  // Binary semaphore
QueueHandle_t xQueue;               // Queue for Task2 -> Task3 communication
volatile int N = 0;                 // Counter variable

// Task 1: Button LED - Detects button press and raises semaphore
void TaskButtonLed(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    
    while (1) {
        if (!LedManager::checkPin(BUTTON_PIN)) {  // Button pressed
            LedManager::on(LED_PIN);
            vTaskDelay(pdMS_TO_TICKS(1000));  // LED on for 1s
            LedManager::off(LED_PIN);
            xSemaphoreGive(xButtonSemaphore);  // Signal to Task 2
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));  // 10ms recurrence
    }
}

// Task 2: Sync Task - Waits for semaphore, increments N, sends data
void TaskSync(void *pvParameters) {
    uint8_t buffer[20];
    while (1) {
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY)) {  // Wait for signal
            N++;
            printf("Task 2: N incremented -> %d\n", N);
            for (int i = 0; i < N; i++) buffer[i] = i + 1;
            xQueueSendToFront(xQueue, buffer, portMAX_DELAY);  // Send to queue
            
            // LED blinks N times
            for (int i = 0; i <= N; i++) {
                LedManager::on(LED_PIN);
                vTaskDelay(pdMS_TO_TICKS(300));
                LedManager::off(LED_PIN);
                vTaskDelay(pdMS_TO_TICKS(500));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));  // Pause between executions
    }
}

// Task 3: Async Task - Reads from queue and displays on Serial
void TaskAsync(void *pvParameters) {
    uint8_t receivedBuffer[20];
    while (1) {
        if (xQueueReceive(xQueue, receivedBuffer, pdMS_TO_TICKS(200))) {
            printf("Task 3: Data received -> ");
            for (int i = 0; i < N; i++) {
                printf("%d ", receivedBuffer[i]);
            }
            printf("\n");
        }
        vTaskDelay(pdMS_TO_TICKS(200));  // 200ms recurrence
    }
}

// Task 4: Statistics Task - Monitors system statistics with mutex protection
typedef struct {
    uint32_t totalDataTransfers;
    uint32_t maxNValue;
} SystemStats_t;

SystemStats_t systemStats;
SemaphoreHandle_t xStatsMutex; // Changed from MutexHandle_t to SemaphoreHandle_t

void TaskStatistics(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {
        if (xSemaphoreTake(xStatsMutex, portMAX_DELAY) == pdTRUE) { // Changed to xSemaphoreTake
            systemStats.totalDataTransfers += N;
            if (N > systemStats.maxNValue) {
                systemStats.maxNValue = N;
            }
            printf("System Statistics:\n");
            printf("- Total data transfers: %lu\n", systemStats.totalDataTransfers);
            printf("- Max N value: %lu\n", systemStats.maxNValue);
            printf("---------------------\n");
            xSemaphoreGive(xStatsMutex); // Changed to xSemaphoreGive
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5000));
    }
}

void setup() {
    Serial.begin(115200);
    IO::init();
    // Initialize mutex for statistics
    xStatsMutex = xSemaphoreCreateMutex(); // Create mutex semaphore
    // Initialize statistics
    memset(&systemStats, 0, sizeof(SystemStats_t));
    // Create synchronization objects
    xButtonSemaphore = xSemaphoreCreateBinary();
    xQueue = xQueueCreate(5, sizeof(uint8_t) * 20);
    
    // Create all tasks
    xTaskCreatePinnedToCore(TaskButtonLed, "Task Button", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(TaskSync, "Task Sync", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(TaskAsync, "Task Async", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(TaskStatistics, "Task Stats", 4096, NULL, 1, NULL, 1);
}

void loop() {
    vTaskDelay(portMAX_DELAY);  // Main task inactive
}