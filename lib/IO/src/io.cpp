#include "io.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <Arduino.h>

// Trim leading and trailing spaces from a string
void io::trim(char *str) {
    // Trim trailing spaces
    int i = strlen(str) - 1;
    while (i >= 0 && isspace((unsigned char)str[i])) {
        str[i--] = '\0';
    }

    // Trim leading spaces
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    memmove(str, start, strlen(start) + 1);
}

// Write a character to the serial port
int io::serial_putchar(char c, FILE* f) {
    Serial.write(c);
    return 0;
}

// Read a character from the serial port
int io::serial_getchar(FILE* f) {
    while (Serial.available() <= 0); // Wait for input
    return Serial.read();
}

// Initialize serial communication
void io::init() {
    // Set up stdin and stdout for serial communication
    FILE* serial_stream = fdevopen(&serial_putchar, &serial_getchar);
    stdin = stdout = serial_stream;
}

// Read a line of input from the serial port
char* io::input() {
    char* input = (char*)malloc(100 * sizeof(char)); // Dynamically allocate memory
    if (input == NULL) {
        return NULL; // Handle allocation failure
    }

    int i = 0;
    char c;

    // Read characters until newline, EOF, or buffer is full
    while ((c = getchar()) != '\n' && c != EOF && i < 99) {
        putchar(c); // Echo the character back
        input[i++] = c;
    }
    input[i] = '\0'; // Null-terminate the string

    trim(input); // Trim leading and trailing spaces
    return input;
}