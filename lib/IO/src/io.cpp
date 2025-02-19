#include "IO.h"
#include <stdIO.h>
#include <string.h>
#include <ctype.h>
#include <Arduino.h>

#define MAX_INPUT 100

int IO::serial_putchar(char c, FILE* f) {
    Serial.write(c);
    return 0;
}

int IO::serial_getchar(FILE* f) {
    while (Serial.available() <= 0);
    char c = Serial.read();  
    Serial.write(c);
    return c;
}
void IO::clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Read until newline
}

void IO::init() {
    Serial.begin(9600);  // Ensure Serial is initialized
    while (!Serial) { delay(10); }  // Wait for Serial to be ready

    static FILE serial_stream;
    fdev_setup_stream(&serial_stream, serial_putchar, serial_getchar, _FDEV_SETUP_RW);
    stdin = stdout = &serial_stream;
}

