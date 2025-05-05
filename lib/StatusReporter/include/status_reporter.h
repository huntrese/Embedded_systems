/**
 * @file status_reporter.h
 * @brief High-level status reporting module
 */

 #ifndef STATUS_REPORTER_H
 #define STATUS_REPORTER_H
 
 #include <stdbool.h>
 
 /**
  * @brief Initialize the status reporter module
  * @return true if initialization successful, false otherwise
  */
 bool status_reporter_init(void);
 
 /**
  * @brief Report current motor status immediately
  */
 void status_report_immediate(void);
 
 /**
  * @brief FreeRTOS task function for periodic status reporting
  * @param pvParameters Task parameters (unused)
  */
 void status_reporter_task(void *pvParameters);
 
 #endif // STATUS_REPORTER_H