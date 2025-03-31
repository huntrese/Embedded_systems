// DisplayManager.cpp
#include "DisplayManager.h"
#include "config.h" // For ProcessedData struct

DisplayManager::DisplayManager()
{
  lastStdioDisplayTime = 0;
}

void DisplayManager::init()
{

  // Print initialization message
  printf("Display Manager Initialized\n");
  printf("LCD: Address 0x%X, %dx%d\n", LCD_ADDRESS, LCD_COLS, LCD_ROWS);
}

void DisplayManager::updateDisplay(const SensorData &data, const SystemStatus &status)
{
  // Update LCD display
  displayOnLCD(data, status);

  // Update Serial at a lower rate to avoid flooding
  unsigned long currentTime = millis();
  if (currentTime - lastStdioDisplayTime >= stdioUpdateInterval)
  {
    displayOnSerial(data, status);
    lastStdioDisplayTime = currentTime;
  }
}

void DisplayManager::displayOnLCD(const SensorData &data, const SystemStatus &status)
{
  // Clear LCD for a new frame (handled by IO module)

  // First line: System state and sound detection
  printf("State: %s\n", getStateString(status.currentState));

  // Second line: Sensor values
  printf("A:%d V:%.2fV D:%d ",
         data.analogValue,
         data.voltage,
         data.digitalValue);
}

void DisplayManager::displayOnSerial(const SensorData &data, const SystemStatus &status)
{
  // Display a structured status report on Serial
  Serial.println("\n--- SYSTEM STATUS REPORT ---");
  Serial.print("Time: ");
  Serial.print(data.timestamp / 1000.0, 1);
  Serial.println("s");

  Serial.print("System State: ");
  Serial.println(getStateString(status.currentState));

  Serial.print("Sound Detections: ");
  Serial.println(status.detectionCount);

  Serial.print("Analog Value: ");
  Serial.print(data.analogValue);
  Serial.print(" (");
  Serial.print(data.voltage, 2);
  Serial.println("V)");

  Serial.print("Digital Trigger: ");
  Serial.println(data.digitalValue == HIGH ? "ACTIVE" : "INACTIVE");

  Serial.print("Threshold Exceeded: ");
  Serial.println(status.thresholdExceeded ? "YES" : "NO");

  Serial.println("---------------------------");
}
static void printData(const ProcessedData &data)
{
  // Format and print the data to the Serial monitor using STDIO printf
  printf("------------------------------------\n");
  printf("Raw ADC:         %d\n", data.rawADC);
  printf("Median Filtered: %.2f\n", data.medianFilteredADC);
  printf("WMA Filtered:    %.2f\n", data.wmaFilteredADC);
  printf("Calculated Vol:  %.3f V\n", data.voltage);
  // printf("Light Level:     %.1f [Unit]\n", data.lightLevel); // If implemented
  printf("------------------------------------\n\n");
}

const char *DisplayManager::getStateString(SystemState state)
{
  switch (state)
  {
  case IDLE:
    return "IDLE";
  case SOUND_DETECTED:
    return "DETECTED";
  case WARNING:
    return "WARNING";
  case ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}