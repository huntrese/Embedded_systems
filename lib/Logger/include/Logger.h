
#pragma once
#include <Arduino.h>

class Logger {
public:
    enum LogLevel {
        TRACE = 0,  // Most detailed, for internal state checks
        DEBUG = 1,  // Function entry/exit and major state changes
        INFO = 2,   // Important operations
        WARN = 3,
        ERROR = 4
    };
    
    static void printf(const char* format, ...);
    static void setLogLevel(LogLevel level) { currentLevel = level; }
    static void log(LogLevel level, const char* format, ...);
    static void beginFunction(const char* function);
    static void endFunction(const char* function);
    
private:
    static LogLevel currentLevel;
    static uint8_t indentLevel;
    static void printIndent();
    static const char* getLogLevelString(LogLevel level);
    static void formatTimestamp(char* buffer, size_t size);
};