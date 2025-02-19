#pragma once

#include <Arduino.h>
#include <stdio.h>

class IO {
public:
    static char* input();
    static void init();
    static void trim(char *str);
    static void clearInputBuffer();
    
private:
    static int serial_putchar(char c, FILE* f);
    static int serial_getchar(FILE* f);
};