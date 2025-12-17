// Simple logger implementation to replace log4cpp
//
#ifndef BIDDING_MODEL_SERVER_INCLUDE_SIMPLE_LOGGER_H_
#define BIDDING_MODEL_SERVER_INCLUDE_SIMPLE_LOGGER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class SimpleLogger {
public:
    SimpleLogger() : level_(LogLevel::INFO) {}
    
    static SimpleLogger& getInstance() {
        static SimpleLogger instance;
        return instance;
    }
    
    void setLevel(LogLevel level) {
        level_ = level;
    }
    
    template<typename... Args>
    void debug(const char* format, Args... args) {
        log(LogLevel::DEBUG, format, args...);
    }
    
    template<typename... Args>
    void info(const char* format, Args... args) {
        log(LogLevel::INFO, format, args...);
    }
    
    template<typename... Args>
    void warn(const char* format, Args... args) {
        log(LogLevel::WARN, format, args...);
    }
    
    template<typename... Args>
    void error(const char* format, Args... args) {
        log(LogLevel::ERROR, format, args...);
    }
    
    template<typename... Args>
    void fatal(const char* format, Args... args) {
        log(LogLevel::FATAL, format, args...);
    }
    
    void shutdown() {
        // No cleanup needed for simple logger
    }
    
private:
    LogLevel level_;
    
    std::string getCurrentTime() {
        time_t now = time(0);
        char buf[80];
        #ifdef _WIN32
        struct tm tm;
        localtime_s(&tm, &now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
        #else
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        #endif
        return std::string(buf);
    }
    
    std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARN: return "WARN";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::FATAL: return "FATAL";
            default: return "UNKNOWN";
        }
    }
    
    template<typename T>
    void formatArg(std::ostringstream& oss, const T& arg) {
        oss << arg;
    }
    
    template<typename T, typename... Args>
    void formatArg(std::ostringstream& oss, const T& arg, Args... args) {
        oss << arg;
        formatArg(oss, args...);
    }
    
    template<typename... Args>
    void log(LogLevel level, const char* format, Args... args) {
        if (level < level_) {
            return;
        }
        
        std::ostringstream oss;
        oss << "[" << getCurrentTime() << "] [" << levelToString(level) << "] ";
        
        // Simple format implementation
        const char* p = format;
        int argIndex = 0;
        
        while (*p) {
            if (*p == '%' && *(p+1) == 'd') {
                oss << std::get<argIndex>(std::make_tuple(args...));
                argIndex++;
                p += 2;
            } else if (*p == '%' && *(p+1) == 's') {
                oss << std::get<argIndex>(std::make_tuple(args...));
                argIndex++;
                p += 2;
            } else if (*p == '%' && *(p+1) == 'f') {
                oss << std::get<argIndex>(std::make_tuple(args...));
                argIndex++;
                p += 2;
            } else {
                oss << *p;
                p++;
            }
        }
        
        std::cout << oss.str() << std::endl;
    }
};

// Global logger instance
extern SimpleLogger* gLogger;

// Helper functions
void initLogger();
SimpleLogger* getLogger();

#endif // BIDDING_MODEL_SERVER_INCLUDE_SIMPLE_LOGGER_H_