//
// Created by Alienson on 27.1.2024.
//

#include "Logger.h"
#include "../core/Configuration.h"

#include <cstdarg>

Logger* Logger::instance = nullptr;
std::mutex Logger::mutex_;

Logger::Logger() {
    logLevel = (LogLevel) Configuration::logLevel;
    appenders.emplace_back("", &std::cout);
    std::string cwd = __FILE__;
    size_t sourceIndex = cwd.find("Winter");
    if (sourceIndex == std::string::npos) {
        std::cout << "Error while initializing logger. Could not find source in path\n";
        sourceIndex = 0;
    }
    cwdOffset = sourceIndex + 7;
}

Logger *Logger::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr){
        instance = new Logger();
    }
    return instance;
}

void Logger::addAppender(std::ostream* stream) {
    appenders.emplace_back("", stream);
}

void Logger::trace(const char* file, const int line, const char* s, ...) const {
    if(logLevel > LOG_LEVEL_TRACE)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(TRACE_STR, file, line, s, argptr);
    va_end(argptr);
}

void Logger::debug(const char* file, const int line, const char* s, ...) const {
    if(logLevel > LOG_LEVEL_DEBUG)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(DEBUG_STR, file, line, s, argptr);
    va_end(argptr);
}

void Logger::info(const char* file, const int line, const char *s, ...) const {
    if(logLevel > LOG_LEVEL_INFO)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(INFO_STR, file, line, s, argptr);
    va_end(argptr);
}

void Logger::warn(const char* file, const int line, const char *s, ...) const {
    if(logLevel > LOG_LEVEL_WARN)
        return;
    va_list argptr;
    va_start(argptr, s);
    log(WARN_STR, file, line, s, argptr);
    va_end(argptr);
}

void Logger::error(const char* file, const int line, const char *s, ...) const {
    va_list argptr;
    va_start(argptr, s);
    log(ERROR_STR, file, line, s, argptr);
    va_end(argptr);
}

void Logger::log(const char *logLevelStr, const char* file, const int line, const char *s, va_list args) const {
    for (auto& appender : appenders){
        va_list args_copy;
        va_copy(args_copy, args);
        appender.writeFormatString(logLevelStr, file + cwdOffset, line);
        appender.write(s, args_copy);
        appender.write("\n");
    }
}
