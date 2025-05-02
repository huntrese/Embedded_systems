/**
 * @file motor_control.h
 * @brief Mid-level motor control module
 */

 #ifndef MOTOR_CONTROL_H
 #define MOTOR_CONTROL_H
 
 #include <stdint.h>
 #include <stdbool.h>
 
 /**
  * @brief Initialize the motor control module
  * @return true if initialization successful, false otherwise
  */
 bool motor_init(void);
 
 /**
  * @brief Set motor power level and direction
  * @param power_percent Value between -100 (full reverse) and 100 (full forward)
  * @return true if successful, false if value out of range
  */
 bool motor_set_power(int8_t power_percent);
 
 /**
  * @brief Stop the motor immediately
  */
 void motor_stop(void);
 
 /**
  * @brief Set motor to maximum power in current direction
  */
 void motor_set_max(void);
 
 /**
  * @brief Increment motor power by 10%
  * @return New power level after increment
  */
 int8_t motor_increment(void);
 
 /**
  * @brief Decrement motor power by 10%
  * @return New power level after decrement
  */
 int8_t motor_decrement(void);
 
 /**
  * @brief Get current motor power level
  * @return Value between -100 and 100
  */
 int8_t motor_get_power(void);
 
 /**
  * @brief Get current motor direction
  * @return 1 for forward, -1 for reverse, 0 for stopped
  */
 int8_t motor_get_direction(void);
 
 #endif // MOTOR_CONTROL_H