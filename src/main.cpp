// main.cpp
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "SensorModule.h"
#include "SignalConditioner.h"
#include "ConversionModule.h"
#include "DisplayModule.h"

// Task handles
TaskHandle_t sensorTaskHandle = NULL;
TaskHandle_t processingTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

// Constants
constexpr int SENSOR_TASK_STACK_SIZE = 2048;
constexpr int PROCESSING_TASK_STACK_SIZE = 2048;
constexpr int DISPLAY_TASK_STACK_SIZE = 2048;
constexpr int SENSOR_TASK_PRIORITY = 3;
constexpr int PROCESSING_TASK_PRIORITY = 2;
constexpr int DISPLAY_TASK_PRIORITY = 1;
constexpr int SENSOR_ACQUISITION_INTERVAL_MS = 10;  // 10ms sensor acquisition
constexpr int PROCESSING_INTERVAL_MS = 20;          // 20ms signal processing
constexpr int DISPLAY_UPDATE_INTERVAL_MS = 500;     // 500ms display update

// Create instances of modules
SensorModule sensorModule;
SignalConditioningModule signalConditioner;
ConversionModule converter;
DisplayModule displayManager;

// Task functions
void sensorAcquisitionTask(void *pvParameters);
void signalProcessingTask(void *pvParameters);
void displayUpdateTask(void *pvParameters);

// Shared data structures with mutex protection
SemaphoreHandle_t dataMutex;
RawSensorData rawData;
ProcessedData processedData;
PhysicalParameter physicalParam;

void setup() {
    // Initialize Serial communication
    Serial.begin(115200);
    delay(1000); // Give some time for serial to initialize
    
    printf("Starting Signal Conditioning Lab...\n");

    // Initialize mutex for data protection
    dataMutex = xSemaphoreCreateMutex();

    // Initialize modules
    sensorModule.init();
    signalConditioner.init();
    converter.init();
    displayManager.init();

    // Initialize the onboard LED
    pinMode(2, OUTPUT);

    // Create tasks with offsets to avoid overlap
    xTaskCreate(sensorAcquisitionTask, "SensorTask", SENSOR_TASK_STACK_SIZE, NULL, SENSOR_TASK_PRIORITY, &sensorTaskHandle);
    xTaskCreate(signalProcessingTask, "ProcessingTask", PROCESSING_TASK_STACK_SIZE, NULL, PROCESSING_TASK_PRIORITY, &processingTaskHandle);
    xTaskCreate(displayUpdateTask, "DisplayTask", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &displayTaskHandle);

    printf("All tasks initialized and started\n");
}

void loop() {
    // Empty - FreeRTOS manages task execution
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Prevent watchdog timeout
}
void sensorAcquisitionTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    printf("Sensor acquisition task started\n");
    
    while (1) {
        RawSensorData newData = sensorModule.readSensor();
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            rawData = newData;
            xSemaphoreGive(dataMutex);
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SENSOR_ACQUISITION_INTERVAL_MS));
    }
}

void signalProcessingTask(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(5));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    printf("Signal processing task started\n");
    
    while (1) {
        RawSensorData currentRawData;
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            currentRawData = rawData;
            xSemaphoreGive(dataMutex);
        }
        ProcessedData filteredData = signalConditioner.applySaltPepperFilter(currentRawData);
        filteredData = signalConditioner.applyWeightedAverageFilter(filteredData);
        PhysicalParameter param = converter.convertToPhysicalParameter(filteredData);
        param = converter.applySaturation(param);
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            processedData = filteredData;
            physicalParam = param;
            xSemaphoreGive(dataMutex);
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(PROCESSING_INTERVAL_MS));
    }
}

void displayUpdateTask(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(50));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    printf("Display update task started\n");
    
    uint32_t reportCounter = 0;
    while (1) {
        ProcessedData currentProcessedData;
        PhysicalParameter currentParam;
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            currentProcessedData = processedData;
            currentParam = physicalParam;
            xSemaphoreGive(dataMutex);
        }
        displayManager.updateDisplay(currentProcessedData, currentParam);
        reportCounter++;
        if (reportCounter >= 10) {  // Every 5 seconds (10 * 500ms)
            printf("\n--- Detailed Signal Report ---\n");
            printf("Raw ADC Value: %d\n", currentProcessedData.rawValue);
            printf("Filtered Value: %d\n", currentProcessedData.filteredValue);
            printf("Voltage: %.3f V\n", currentProcessedData.voltage);
            printf("Physical Parameter: %.2f %s\n", currentParam.value, currentParam.unit);
            printf("Signal Quality: %s\n", signalConditioner.getSignalQualityDescription());
            printf("----------------------------\n\n");
            reportCounter = 0;
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(DISPLAY_UPDATE_INTERVAL_MS));
    }
}
