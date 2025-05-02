/**
 * @file motor_control.c
 * @brief Implementation of mid-level motor control module
 */

 #include "motor_control.h"
 #include "l298_driver.h"
 
 // Current motor state
 static int8_t _current_power = 0;     // -100 to 100
 static int8_t _current_direction = 0; // -1, 0, 1
 
 // Power increment/decrement step
 #define POWER_STEP 10
 
 bool motor_init(void) {
     // Configure L298 pins
     l298_config_t config = {
         .in1_pin = 26,           // PWM pin for IN1
         .in2_pin = 27,           // PWM pin for IN2
         .pwm_frequency = 5000,   // 5kHz PWM frequency
         .pwm_channel1 = 0,       // PWM channel 0 for IN1
         .pwm_channel2 = 1,       // PWM channel 1 for IN2
         .pwm_resolution = 8      // 8-bit resolution (0-255)
     };
     
     return l298_init(&config);
 }
 
 bool motor_set_power(int8_t power_percent) {
     // Validate power range
     if (power_percent < -100 || power_percent > 100) {
         return false;
     }
     
     // Update current power and direction
     _current_power = (power_percent < 0) ? -power_percent : power_percent;
     _current_direction = (power_percent > 0) ? 1 : (power_percent < 0) ? -1 : 0;
     
     // Convert percentage to PWM value (0-255)
     int16_t pwm_value = (_current_power * 255) / 100;
     
     // Apply direction
     if (_current_direction < 0) {
         pwm_value = -pwm_value;
     }
     
     // Set power through L298 driver
     l298_set_power(pwm_value);
     
     return true;
 }
 
 void motor_stop(void) {
     l298_stop();
     _current_power = 0;
     _current_direction = 0;
 }
 
 void motor_set_max(void) {
     if (_current_direction == 0) {
         // If currently stopped, use forward direction
         motor_set_power(100);
     } else {
         // Otherwise use current direction at max power
         motor_set_power(_current_direction * 100);
     }
 }
 
 int8_t motor_increment(void) {
     int8_t new_power = _current_power + POWER_STEP;
     
     // Handle direction
     if (new_power > 100) new_power = 100;
     
     int8_t power_with_direction = (_current_direction >= 0) ? new_power : -new_power;
     motor_set_power(power_with_direction);
     
     return power_with_direction;
 }
 
 int8_t motor_decrement(void) {
     int8_t new_power = _current_power - POWER_STEP;
     
     // Don't go below 0
     if (new_power < 0) new_power = 0;
     
     int8_t power_with_direction = (_current_direction >= 0) ? new_power : -new_power;
     motor_set_power(power_with_direction);
     
     return power_with_direction;
 }
 
 int8_t motor_get_power(void) {
     return _current_direction * _current_power;
 }
 
 int8_t motor_get_direction(void) {
     return _current_direction;
 }