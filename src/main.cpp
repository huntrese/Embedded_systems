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
    
    Serial.println("Starting Signal Conditioning Lab...");

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
    xTaskCreate(
        sensorAcquisitionTask,  // Task function
        "SensorTask",           // Task name
        SENSOR_TASK_STACK_SIZE, // Stack size
        NULL,                   // Parameters
        SENSOR_TASK_PRIORITY,   // Priority
        &sensorTaskHandle       // Task handle
    );

    xTaskCreate(
        signalProcessingTask,    // Task function
        "ProcessingTask",        // Task name
        PROCESSING_TASK_STACK_SIZE, // Stack size
        NULL,                    // Parameters
        PROCESSING_TASK_PRIORITY,// Priority
        &processingTaskHandle    // Task handle
    );

    xTaskCreate(
        displayUpdateTask,       // Task function
        "DisplayTask",           // Task name
        DISPLAY_TASK_STACK_SIZE, // Stack size
        NULL,                    // Parameters
        DISPLAY_TASK_PRIORITY,   // Priority
        &displayTaskHandle       // Task handle
    );

    Serial.println("All tasks initialized and started");
}

void loop() {
    // Empty - FreeRTOS manages task execution
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Prevent watchdog timeout
}

// Task for sensor data acquisition
void sensorAcquisitionTask(void *pvParameters) {
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    Serial.println("Sensor acquisition task started");
    
    while (1) {
        // Get sensor readings
        RawSensorData newData = sensorModule.readSensor();
        
        // Protect shared data with mutex
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            rawData = newData;
            xSemaphoreGive(dataMutex);
        }
        
        // Precise timing using vTaskDelayUntil
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SENSOR_ACQUISITION_INTERVAL_MS));
    }
}

// Task for signal processing
void signalProcessingTask(void *pvParameters) {
    // Add a small offset to avoid task overlap with acquisition
    vTaskDelay(pdMS_TO_TICKS(5));
    
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    Serial.println("Signal processing task started");
    
    while (1) {
        RawSensorData currentRawData;
        
        // Get the latest raw data with mutex protection
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            currentRawData = rawData;
            xSemaphoreGive(dataMutex);
        }
        
        // Apply salt and pepper filter
        ProcessedData filteredData = signalConditioner.applySaltPepperFilter(currentRawData);
        
        // Apply weighted average filter
        filteredData = signalConditioner.applyWeightedAverageFilter(filteredData);
        
        // Convert ADC to voltage and then to physical parameter
        PhysicalParameter param = converter.convertToPhysicalParameter(filteredData);
        
        // Apply saturation to ensure values are in valid range
        param = converter.applySaturation(param);
        
        // Update processed data with mutex protection
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            processedData = filteredData;
            physicalParam = param;
            xSemaphoreGive(dataMutex);
        }
        
        // Precise timing
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(PROCESSING_INTERVAL_MS));
    }
}

// Display update task
void displayUpdateTask(void *pvParameters) {
    // Add a larger offset to avoid task overlap
    vTaskDelay(pdMS_TO_TICKS(50));

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    Serial.println("Display update task started");
    
    uint32_t reportCounter = 0;
    
    while (1) {
        ProcessedData currentProcessedData;
        PhysicalParameter currentParam;
        
        // Get the latest processed data with mutex protection
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            currentProcessedData = processedData;
            currentParam = physicalParam;
            xSemaphoreGive(dataMutex);
        }
        
        // Update display with current data
        displayManager.updateDisplay(currentProcessedData, currentParam);
        
        // Periodic detailed report
        reportCounter++;
        if (reportCounter >= 10) {  // Every 5 seconds (10 * 500ms)
            Serial.println("\n--- Detailed Signal Report ---");
            Serial.print("Raw ADC Value: ");
            Serial.println(currentProcessedData.rawValue);
            Serial.print("Filtered Value: ");
            Serial.println(currentProcessedData.filteredValue);
            Serial.print("Voltage: ");
            Serial.print(currentProcessedData.voltage, 3);
            Serial.println(" V");
            Serial.print("Physical Parameter: ");
            Serial.print(currentParam.value, 2);
            Serial.print(" ");
            Serial.println(currentParam.unit);
            Serial.print("Signal Quality: ");
            Serial.println(signalConditioner.getSignalQualityDescription());
            Serial.println("----------------------------\n");
            reportCounter = 0;
        }
        
        // Precise timing
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(DISPLAY_UPDATE_INTERVAL_MS));
    }
}