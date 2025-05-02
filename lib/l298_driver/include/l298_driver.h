/**
 * @file l298_driver.h
 * @brief Low-level driver for L298 motor controller using PWM on IN1/IN2
 */

 #ifndef L298_DRIVER_H
 #define L298_DRIVER_H
 
 #include <stdint.h>
 #include <stdbool.h>
 
 /**
  * @brief L298 driver configuration structure
  */
 typedef struct {
     uint8_t in1_pin;        // Input 1 pin (PWM)
     uint8_t in2_pin;        // Input 2 pin (PWM)
     uint16_t pwm_frequency; // PWM frequency in Hz
     uint8_t pwm_channel1;   // PWM channel for IN1
     uint8_t pwm_channel2;   // PWM channel for IN2
     uint8_t pwm_resolution; // PWM resolution in bits
 } l298_config_t;
 
 /**
  * @brief Initialize the L298 driver
  * @param config Driver configuration
  * @return true if initialization successful, false otherwise
  */
 bool l298_init(const l298_config_t *config);
 
 /**
  * @brief Set motor power and direction
  * @param power Value between -255 (full reverse) and 255 (full forward)
  */
 void l298_set_power(int16_t power);
 
 /**
  * @brief Stop the motor (brake mode)
  */
 void l298_stop(void);
 
 #endif // L298_DRIVER_H