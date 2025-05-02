#ifndef STDIO_REDIRECT_H
#define STDIO_REDIRECT_H

#include <Arduino.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Function to receive characters from stdin
static int uart_getchar(FILE *stream) {
  // Wait for data to be available
  while (!Serial.available());
  return Serial.read();
}

// Function to send characters to stdout
static int uart_putchar(char c, FILE *stream) {
  if (c == '\n') {
    Serial.write('\r');
  }
  Serial.write(c);
  return 0;
}


// Function to set up STDIO redirection
void setupStdio() {
  // Initialize serial first
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  // Set up stdout and stdin
  static FILE uartout = {0};
  static FILE uartin = {0};
  
  // Configure stdout
  fdev_setup_stream(&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;
  
  // Configure stdin
  fdev_setup_stream(&uartin, NULL, uart_getchar, _FDEV_SETUP_READ);
  stdin = &uartin;
  
  // Set up stderr to also go to stdout
  stderr = stdout;
}

#ifdef __cplusplus
}
#endif

#endif // STDIO_REDIRECT_H