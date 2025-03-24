// main.cpp
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "SoundSensor.h"
#include "SignalManager.h"
#include "DisplayManager.h"
#include "LedManager.h"

// Task handles
TaskHandle_t sensorTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

// Constants
constexpr int SENSOR_TASK_STACK_SIZE = 2048;
constexpr int DISPLAY_TASK_STACK_SIZE = 2048;
constexpr int SENSOR_TASK_PRIORITY = 2;
constexpr int DISPLAY_TASK_PRIORITY = 1;
constexpr int SENSOR_ACQUISITION_INTERVAL_MS = 2; // 100ms sensor acquisition
constexpr int DISPLAY_UPDATE_INTERVAL_MS = 500;     // 500ms display update

// Create instances of sensor and signal manager
SoundSensor soundSensor;
SignalManager signalManager;
DisplayManager displayManager;
std::map<int, std::map<String, int>> blinkList;

// Task functions
void sensorAcquisitionTask(void *pvParameters);
void displayUpdateTask(void *pvParameters);

void setup()
{
    // Initialize Serial communication
    Serial.begin(115200);
    delay(1000); // Give some time for serial to initialize

    // Initialize IO module for LCD and Serial redirection
    IO::init(IO::SERIAL_MODE);

    // Initialize modules
    soundSensor.init();
    signalManager.init();
    displayManager.init();

    // Initialize LED blink list and configure timer
    LedManager::blink_init(blinkList, 50); // 50ms base timer interval

    // Initialize the onboard LED
    pinMode(2, OUTPUT);

    // Create tasks
    xTaskCreate(
        sensorAcquisitionTask,  // Task function
        "SensorTask",           // Task name
        SENSOR_TASK_STACK_SIZE, // Stack size
        NULL,                   // Parameters
        SENSOR_TASK_PRIORITY,   // Priority
        &sensorTaskHandle       // Task handle
    );

    xTaskCreate(
        displayUpdateTask,       // Task function
        "DisplayTask",           // Task name
        DISPLAY_TASK_STACK_SIZE, // Stack size
        NULL,                    // Parameters
        DISPLAY_TASK_PRIORITY,   // Priority
        &displayTaskHandle       // Task handle
    );
}

void loop()
{
    // Empty - FreeRTOS manages task execution
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Prevent watchdog timeout
}
void sensorAcquisitionTask(void *pvParameters)
{
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    bool previousSoundState = false;
    bool ledState = false;
    TickType_t lastToggleTime = 0;
    const TickType_t DEBOUNCE_DELAY_MS = pdMS_TO_TICKS(50); // 50ms debounce delay
    
    while (1)
    {
        // Get sensor readings
        SensorData data = soundSensor.readSensor();
        bool currentSoundState = (signalManager.getSystemStatus().currentState == SOUND_DETECTED);
        
        // Check for rising edge with debounce
        if (data.digitalValue)
        {
            TickType_t currentTime = xTaskGetTickCount();
            
            // Only toggle if enough time has passed since last toggle
            if ((currentTime - lastToggleTime) > DEBOUNCE_DELAY_MS)
            {
                ledState = !ledState;
                digitalWrite(2, ledState);
                Serial.println("🔥 Sound Detected! Toggling LED");
                lastToggleTime = currentTime;
            }
        }
        
        previousSoundState = currentSoundState;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SENSOR_ACQUISITION_INTERVAL_MS));
    }
}

// Display update task
void displayUpdateTask(void *pvParameters)
{
    // Add a small offset to avoid task overlap
    vTaskDelay(pdMS_TO_TICKS(50));

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        // Get current signal status
        SensorData data = signalManager.getCurrentData();
        SystemStatus status = signalManager.getSystemStatus();

        // Update display
        displayManager.updateDisplay(data, status);

        // Precise timing using vTaskDelayUntil
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(DISPLAY_UPDATE_INTERVAL_MS));
    }
}