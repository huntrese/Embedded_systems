#include "io.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <Arduino.h>

int io::serial_putchar(char c, FILE* f) {
    Serial.write(c);
    return 0;
}

int io::serial_getchar(FILE* f) {
    while (Serial.available() <= 0);
    return Serial.read();
}

void io::init() {
    FILE* serial_stream = fdevopen(&serial_putchar, &serial_getchar);
    stdin = stdout = serial_stream;
}

void io::trim(char *str) {
    int i = strlen(str) - 1;
    while (i >= 0 && isspace((unsigned char)str[i])) {
        str[i--] = '\0';
    }

    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    memmove(str, start, strlen(start) + 1);
}

char* io::input() {
    static char input[100];
    if (input == NULL) {
        return NULL;
    }

    char c = 0;
    int i = 0;

    while (scanf("%c", &c) != EOF && c != '\n' && i < 99) {
        if (c == '\b' && i > 0) {
            printf("\b \b");
            i--;
        } else if (c != '\b') {
            printf("%c", c);
            input[i++] = c;
        }
    }
    
    input[i] = '\0';
    trim(input);
    return input;
}