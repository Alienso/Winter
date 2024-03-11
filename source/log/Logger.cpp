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
    string cwd = __FILE__;
    size_t sourceIndex = cwd.find("log");
    if (sourceIndex == string::npos) {
        cout << "Error while initializing logger. Could not find source in path\n";
        sourceIndex = 0;
    }
    cwdOffset = sourceIndex;
}

Logger *Logger::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr)
    {
        instance = new Logger();
    }
    return instance;
}

void Logger::trace(const char* file, int line, const char* s, ...) const {
    if(logLevel > LOG_LEVEL_TRACE)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(TRACE_STR, file, line, s, argptr);
    va_end(argptr);
}

void Logger::debug(const char* file, int line, const char* s, ...) const {
    if(logLevel > LOG_LEVEL_DEBUG)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(DEBUG_STR, file, line, s, argptr);
    va_end(argptr);
}

void Logger::info(const char* file, int line, const char *s, ...) const {
    if(logLevel > LOG_LEVEL_INFO)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(INFO_STR, file, line, s, argptr);
    va_end(argptr);
}

void Logger::warn(const char* file, int line, const char *s, ...) const {
    if(logLevel > LOG_LEVEL_WARN)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(WARN_STR, file, line, s, argptr);
    va_end(argptr);
}

void Logger::error(const char* file, int line, const char *s, ...) const {
    va_list argptr;
    va_start(argptr, s);
    log(ERROR_STR, file + cwdOffset, line, s, argptr);
    va_end(argptr);
}

void Logger::log(const char *logLevelStr, const char* file, int line, const char *s, va_list args) const {
    for (auto& appender : appenders){
        printf("%s %s:%d\t", logLevelStr, file + cwdOffset, line);
        appender.write(s, args);
        printf("\n");
    }
}
