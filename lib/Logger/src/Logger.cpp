#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>


void Logger::printf(const char* format, ...) {
    char buffer[128];  // Adjust based on available RAM
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Serial.println(buffer);
}

