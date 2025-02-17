#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>

Logger::LogLevel Logger::currentLevel = Logger::DEBUG;
uint8_t Logger::indentLevel = 0;

void Logger::printf(const char* format, ...) {
    char buffer[128];  // Adjust based on available RAM
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Serial.println(buffer);
}

void Logger::printIndent() {
    for (uint8_t i = 0; i < indentLevel; i++) {
        Serial.print("  ");
    }
}

void Logger::formatTimestamp(char* buffer, size_t size) {
    unsigned long ms = millis();
    snprintf(buffer, size, "[%lu.%03lu]", ms / 1000, ms % 1000);
}

const char* Logger::getLogLevelString(LogLevel level) {
    switch (level) {
        case TRACE: return "TRACE";
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARN: return "WARN";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const char* format, ...) {
    if (level < currentLevel) return;
    
    char timestamp[16];
    formatTimestamp(timestamp, sizeof(timestamp));
    
    Serial.print(timestamp);
    Serial.print(" ");
    Serial.print(getLogLevelString(level));
    Serial.print(" ");
    printIndent();
    
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    Serial.println(buffer);
}

void Logger::beginFunction(const char* function) {
    if (DEBUG >= currentLevel) {
        log(DEBUG, "→ %s", function);
    }
    indentLevel++;
}

void Logger::endFunction(const char* function) {
    indentLevel--;
    if (DEBUG >= currentLevel) {
        log(DEBUG, "← %s", function);
    }
}