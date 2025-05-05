/**
 * @file command_interface.h
 * @brief High-level command interface for user interaction
 */

 #ifndef COMMAND_INTERFACE_H
 #define COMMAND_INTERFACE_H
 
 #include <stdbool.h>
 
 /**
  * @brief Initialize the command interface
  * @return true if initialization successful, false otherwise
  */
 bool command_interface_init(void);
 
 /**
  * @brief Process user commands from STDIO
  * @return true if a command was processed, false otherwise
  */
 bool command_process(void);
 
 /**
  * @brief FreeRTOS task function for command processing
  * @param pvParameters Task parameters (unused)
  */
 void command_task(void *pvParameters);
 
 #endif // COMMAND_INTERFACE_H