#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" // Required for mutex
// #include "config.h"
#include "SensorReader.h"
#include "SignalConditioner.h"
#include "DisplayManager.h"
// #include "IO.h" // Include if using a specific IO redirection class

// --- Task Handles ---
TaskHandle_t acquisitionTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

// --- Shared Data & Mutex ---
ProcessedData_float sharedSensorData; // Or int, depending on your use case
SemaphoreHandle_t dataMutex;

// --- Peripheral Instances ---
SensorReader ldrSensor(LDR_PIN);
SignalConditioner signalProcessor;
// DisplayManager is static, no instance needed

// --- Task Function Prototypes ---
void acquisitionTask(void *pvParameters);
void displayTask(void *pvParameters);

void setup() {
    Serial.begin(115200);
    delay(1000); // Wait for serial monitor
    printf("System Setup Started...\n");

    // Initialize IO if needed (assuming Serial.begin handles printf redirection)
    // IO::init(IO::SERIAL_MODE);

    // Initialize Modules
    ldrSensor.init();
    signalProcessor.init();
    DisplayManager::init(); // Static init

    // Create Mutex for shared data
    dataMutex = xSemaphoreCreateMutex();
    if (dataMutex == NULL) {
        printf("ERROR: Failed to create data mutex!\n");
        // Handle error - perhaps halt or loop infinitely
        while(1);
    }

    // Create Acquisition Task
    xTaskCreate(
        acquisitionTask,
        "AcquisitionTask",
        ACQUISITION_TASK_STACK_SIZE,
        NULL, // No parameters passed
        ACQUISITION_TASK_PRIORITY,
        &acquisitionTaskHandle
    );

    // Create Display Task
    xTaskCreate(
        displayTask,
        "DisplayTask",
        DISPLAY_TASK_STACK_SIZE,
        NULL, // No parameters passed
        DISPLAY_TASK_PRIORITY,
        &displayTaskHandle
    );

    printf("Setup complete. Tasks created.\n");
    // No need to explicitly start the scheduler, it starts automatically
    // after setup() returns in Arduino FreeRTOS implementations.
}

void loop() {
    // Keep loop empty or add minimal watchdog feeding if necessary
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
}

// --- Task Implementations ---

void acquisitionTask(void *pvParameters) {
    printf("Acquisition Task started.\n");
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(ACQUISITION_INTERVAL_MS);

    // Initialize the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        // 1. Read raw sensor value
        int rawValue = ldrSensor.readRawValue();

        // 2. Process the value (filter, convert, saturate)
        ProcessedData_float currentProcessedData = signalProcessor.processNewRawValue(rawValue);

        // 3. Update shared data structure (protected by mutex)
        if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
            sharedSensorData = currentProcessedData; // Copy data
            xSemaphoreGive(dataMutex);
        } else {
             printf("ACQ_TASK: Failed to take mutex!\n");
             // Handle error - maybe skip update this cycle?
        }

        // 4. Wait for the next cycle using vTaskDelayUntil for precise timing
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void displayTask(void *pvParameters) {
    printf("Display Task starting after offset.\n");
    // Apply initial offset to avoid starting at the exact same time as acquisition
    vTaskDelay(pdMS_TO_TICKS(DISPLAY_TASK_OFFSET_MS));
    printf("Display Task started.\n");

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(DISPLAY_INTERVAL_MS);

    // Initialize the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        ProcessedData_float dataToDisplay;

        // 1. Read shared data structure (protected by mutex)
        if (xSemaphoreTake(dataMutex, portMAX_DELAY) == pdTRUE) {
            dataToDisplay = sharedSensorData; // Copy data
            xSemaphoreGive(dataMutex);

            // 2. Print the data using DisplayManager
            DisplayManager::printData(dataToDisplay);

        } else {
             printf("DISP_TASK: Failed to take mutex!\n");
             // Handle error - maybe skip display this cycle?
        }

        // 3. Wait for the next cycle using vTaskDelayUntil
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}