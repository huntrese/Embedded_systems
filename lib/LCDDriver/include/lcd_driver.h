/**
 * @file lcd_i2c_driver.h
 * @brief Low-level driver for I2C LCD display
 */

 #ifndef LCD_I2C_DRIVER_H
 #define LCD_I2C_DRIVER_H
 
 #include <stdint.h>
 #include <stdbool.h>
 
 /**
  * @brief LCD I2C configuration structure
  */
 typedef struct {
     uint8_t i2c_addr;    // I2C address of the LCD
     uint8_t columns;     // Number of columns
     uint8_t rows;        // Number of rows
     uint8_t sda_pin;     // SDA pin (optional, set to 0 to use default)
     uint8_t scl_pin;     // SCL pin (optional, set to 0 to use default)
 } lcd_i2c_config_t;
 
 /**
  * @brief Initialize the I2C LCD display
  * @param config LCD configuration
  * @return true if initialization successful, false otherwise
  */
 bool lcd_i2c_init(const lcd_i2c_config_t *config);
 
 /**
  * @brief Clear the LCD display
  */
 void lcd_i2c_clear(void);
 
 /**
  * @brief Set the cursor position
  * @param column Column position (0-based)
  * @param row Row position (0-based)
  */
 void lcd_i2c_set_cursor(uint8_t column, uint8_t row);
 
 /**
  * @brief Print a string to the LCD at current cursor position
  * @param text Text string to print
  */
 void lcd_i2c_print(const char *text);
 
 /**
  * @brief Print a formatted string to the LCD at current cursor position
  * @param format Format string (printf style)
  * @param ... Variable arguments
  */
 void lcd_i2c_printf(const char *format, ...);
 
 /**
  * @brief Turn on or off the backlight
  * @param on true to turn on, false to turn off
  */
 void lcd_i2c_backlight(bool on);
 
 /**
  * @brief Create a custom character in LCD memory
  * @param location Memory location (0-7)
  * @param charmap Character bitmap data (8 bytes)
  */
 void lcd_i2c_create_char(uint8_t location, const uint8_t charmap[]);
 
 #endif // LCD_I2C_DRIVER_H