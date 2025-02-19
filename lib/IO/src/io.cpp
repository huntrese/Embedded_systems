#include "IO.h"
#include <stdIO.h>
#include <string.h>
#include <ctype.h>
#include <Arduino.h>

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
    while ((c = getchar()) != '\n' && c != EOF);
}

void IO::init() {
    Serial.begin(9600);  
    while (!Serial) { delay(10); }  

    static FILE serial_stream;
    fdev_setup_stream(&serial_stream, serial_putchar, serial_getchar, _FDEV_SETUP_RW);
    stdin = stdout = &serial_stream;
}

void IO::trimWhitespace(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }

    char *end = str + strlen(str) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    *(end + 1) = '\0';

    if (start != str) {
        memmove(str, start, end - start + 2); 
    }
}
