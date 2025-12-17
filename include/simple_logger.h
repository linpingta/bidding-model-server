#ifndef BIDDING_MODEL_SERVER_SIMPLE_LOGGER_H
#define BIDDING_MODEL_SERVER_SIMPLE_LOGGER_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <stdarg.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

class SimpleLogger {
public:
    static SimpleLogger& getInstance() {
        static SimpleLogger instance;
        return instance;
    }

    void setLevel(LogLevel level) {
        currentLevel = level;
    }

    void debug(const std::string& message) {
        if (currentLevel <= LOG_DEBUG) {
            log(LOG_DEBUG, message);
        }
    }

    void info(const std::string& message) {
        if (currentLevel <= LOG_INFO) {
            log(LOG_INFO, message);
        }
    }

    void warn(const std::string& message) {
        if (currentLevel <= LOG_WARN) {
            log(LOG_WARN, message);
        }
    }

    void error(const std::string& message) {
        if (currentLevel <= LOG_ERROR) {
            log(LOG_ERROR, message);
        }
    }

    void fatal(const std::string& message) {
        if (currentLevel <= LOG_FATAL) {
            log(LOG_FATAL, message);
        }
    }

    // Format versions
    void debug(const char* format, ...) {
        if (currentLevel <= LOG_DEBUG) {
            va_list args;
            va_start(args, format);
            logFormatted(LOG_DEBUG, format, args);
            va_end(args);
        }
    }

    void info(const char* format, ...) {
        if (currentLevel <= LOG_INFO) {
            va_list args;
            va_start(args, format);
            logFormatted(LOG_INFO, format, args);
            va_end(args);
        }
    }

    void warn(const char* format, ...) {
        if (currentLevel <= LOG_WARN) {
            va_list args;
            va_start(args, format);
            logFormatted(LOG_WARN, format, args);
            va_end(args);
        }
    }

    void error(const char* format, ...) {
        if (currentLevel <= LOG_ERROR) {
            va_list args;
            va_start(args, format);
            logFormatted(LOG_ERROR, format, args);
            va_end(args);
        }
    }

    void fatal(const char* format, ...) {
        if (currentLevel <= LOG_FATAL) {
            va_list args;
            va_start(args, format);
            logFormatted(LOG_FATAL, format, args);
            va_end(args);
        }
    }

    void shutdown() {
        // No cleanup needed
    }

private:
    SimpleLogger() : currentLevel(LOG_INFO) {}
    
    void log(LogLevel level, const std::string& message) {
        time_t now = time(0);
        char timeBuffer[80];
        
        #ifdef _WIN32
        struct tm localTime;
        localtime_s(&localTime, &now);
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &localTime);
        #else
        struct tm* localTime = localtime(&now);
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", localTime);
        #endif
        
        std::string levelStr;
        switch (level) {
            case LOG_DEBUG: levelStr = "DEBUG";
                break;
            case LOG_INFO: levelStr = "INFO";
                break;
            case LOG_WARN: levelStr = "WARN";
                break;
            case LOG_ERROR: levelStr = "ERROR";
                break;
            case LOG_FATAL: levelStr = "FATAL";
                break;
            default: levelStr = "UNKNOWN";
        }
        
        std::cout << "[" << timeBuffer << "] [" << levelStr << "] " << message << std::endl;
    }
    
    void logFormatted(LogLevel level, const char* format, va_list args) {
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), format, args);
        log(level, std::string(buffer));
    }
    
    LogLevel currentLevel;
};

extern SimpleLogger* gLogger;

void initLogger();
SimpleLogger* getLogger();

#endif
