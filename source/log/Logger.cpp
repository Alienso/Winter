//
// Created by Alienson on 27.1.2024..
//

#include "Logger.h"
#include "../Configuration.h"
#include <cstdarg>

using namespace std;

Logger* Logger::instance = nullptr;
mutex Logger::mutex_;

Logger::Logger() {
    logLevel = (LogLevel) Configuration::logLevel;
    appenders.emplace_back("", &cout);
}

Logger *Logger::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr)
    {
        instance = new Logger();
    }
    return instance;
}

void Logger::trace(const char* file, int line, const char* s, ...) {
    if(logLevel > LOG_LEVEL_TRACE)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(TRACE_STR, s, argptr);
    va_end(argptr);
}

void Logger::debug(const char* file, int line, const char* s, ...) {
    if(logLevel > LOG_LEVEL_DEBUG)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(DEBUG_STR, s, argptr);
    va_end(argptr);
}

void Logger::info(const char* file, int line, const char *s, ...) {
    if(logLevel > LOG_LEVEL_INFO)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(INFO_STR, s, argptr);
    va_end(argptr);
}

void Logger::warn(const char* file, int line, const char *s, ...) {
    if(logLevel > LOG_LEVEL_WARN)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(WARN_STR, s, argptr);
    va_end(argptr);
}

void Logger::error(const char* file, int line, const char *s, ...) {
    va_list argptr;
    va_start(argptr, s);
    log(ERROR_STR, s, argptr);
    va_end(argptr);
}

void Logger::log(const char *logLevel, const char *s, va_list args) {
    for (auto appender : appenders){
        printf(logLevel);
        appender.write(s, args);
        printf("\n");
    }
}
