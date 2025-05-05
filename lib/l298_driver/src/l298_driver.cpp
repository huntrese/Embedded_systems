/**
 * @file l298_driver.c
 * @brief Implementation of low-level L298 driver functions using PWM on IN1/IN2
 */

 #include "l298_driver.h"
 #include "Arduino.h"
 
 // Static configuration storage
 static l298_config_t _config;
 
 bool l298_init(const l298_config_t *config) {
     if (!config) return false;
     
     // Store configuration
     _config = *config;
     
     // Configure pins
     pinMode(_config.in1_pin, OUTPUT);
     pinMode(_config.in2_pin, OUTPUT);
     
     // Initialize PWM channels
     #ifdef ESP32
         // ESP32 specific PWM setup
         ledcSetup(_config.pwm_channel1, _config.pwm_frequency, _config.pwm_resolution);
         ledcSetup(_config.pwm_channel2, _config.pwm_frequency, _config.pwm_resolution);
         ledcAttachPin(_config.in1_pin, _config.pwm_channel1);
         ledcAttachPin(_config.in2_pin, _config.pwm_channel2);
     #endif
     
     // Initialize with motor stopped
     l298_stop();
     
     return true;
 }
 
 void l298_set_power(int16_t power) {
     uint8_t pwm_value = (power < 0) ? -power : power;
     
     // Limit PWM value to maximum
     if (pwm_value > 255) pwm_value = 255;
     
     #ifdef ESP32
         // ESP32 PWM control
         uint32_t max_duty = (1 << _config.pwm_resolution) - 1;
         uint32_t scaled_duty = (pwm_value * max_duty) / 255;
         
         if (power > 0) {
             // Forward: IN1=PWM, IN2=0
             ledcWrite(_config.pwm_channel1, scaled_duty);
             ledcWrite(_config.pwm_channel2, 0);
         } else if (power < 0) {
             // Reverse: IN1=0, IN2=PWM
             ledcWrite(_config.pwm_channel1, 0);
             ledcWrite(_config.pwm_channel2, scaled_duty);
         } else {
             // Stop: Both outputs off
             ledcWrite(_config.pwm_channel1, 0);
             ledcWrite(_config.pwm_channel2, 0);
         }
     #else
         // Arduino PWM control
         if (power > 0) {
             // Forward: IN1=PWM, IN2=0
             analogWrite(_config.in1_pin, pwm_value);
             analogWrite(_config.in2_pin, 0);
         } else if (power < 0) {
             // Reverse: IN1=0, IN2=PWM
             analogWrite(_config.in1_pin, 0);
             analogWrite(_config.in2_pin, pwm_value);
         } else {
             // Stop: Both outputs off
             analogWrite(_config.in1_pin, 0);
             analogWrite(_config.in2_pin, 0);
         }
     #endif
 }
 
 void l298_stop(void) {
     #ifdef ESP32
         ledcWrite(_config.pwm_channel1, 0);
         ledcWrite(_config.pwm_channel2, 0);
     #else
         analogWrite(_config.in1_pin, 0);
         analogWrite(_config.in2_pin, 0);
     #endif
 }