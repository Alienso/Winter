//
// Created by Alienson on 27.1.2024.
//

#ifndef WINTER_LOGGER_H
#define WINTER_LOGGER_H

/**
 * For given set of Appenders, logs to each of them
 * Each appender is a stream with printing format
 */

#include <string>
#include <vector>
#include <mutex>

#include "Appender.h"

#define wtLogTrace(format, ...) Logger::getInstance()->trace(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogInfo(format, ...) Logger::getInstance()->info(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogDebug(format, ...) Logger::getInstance()->debug(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogWarn(format, ...) Logger::getInstance()->warn(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogError(format, ...) Logger::getInstance()->error(__FILE__, __LINE__, format, ##__VA_ARGS__)


enum LogLevel{
    LOG_LEVEL_TRACE,LOG_LEVEL_DEBUG,LOG_LEVEL_INFO,LOG_LEVEL_WARN,LOG_LEVEL_ERROR
};

class Logger {

public:

    Logger(Logger& other) = delete;
    void operator=(const Logger&) = delete;
    static Logger* getInstance();

    void addAppender(std::ostream* stream);

    void trace(const char* file, int line, const char* s, ...) const;
    void debug(const char* file, int line, const char* s, ...) const;
    void info(const char* file, int line, const char* s, ...) const;
    void warn(const char* file, int line, const char* s, ...) const;
    void error(const char* file, int line, const char* s, ...) const;

    template<typename T>
    void trace(const char* file, const int line, const char *s, const std::vector<T> vec) const {
        if(logLevel > LOG_LEVEL_TRACE)
            return;
        logArray(TRACE_STR, file + cwdOffset, line, s, vec.data(), vec.size());
    }

    template<typename T>
    void debug(const char* file, const int line, const char *s, const std::vector<T> vec) const {
        if(logLevel > LOG_LEVEL_DEBUG)
            return;
        logArray(DEBUG_STR, file + cwdOffset, line, s, vec.data(), vec.size());
    }

    template<typename T>
    void info(const char* file, const int line, const char *s, const std::vector<T> vec) const {
        if(logLevel > LOG_LEVEL_INFO)
            return;
        logArray(INFO_STR, file + cwdOffset, line, s, vec.data(), vec.size());
    }

    template<typename T>
    void warn(const char* file, int line, const char *s, const std::vector<T> vec) const {
        if(logLevel > LOG_LEVEL_WARN)
            return;
        logArray(WARN_STR, file + cwdOffset, line, s, vec.data(), vec.size());
    }

    template<typename T>
    void error(const char* file, const int line, const char *s, const std::vector<T> vec) const {
        logArray(ERROR_STR, file + cwdOffset, line, s, vec.data(), vec.size());
    }

    template<typename T>
    void trace(const char* file, const int line, const char *s, const T *array, const int n) const {
        if(logLevel > LOG_LEVEL_TRACE)
            return;
        logArray(TRACE_STR, file + cwdOffset, line, s, array, n);
    }

    template<typename T>
    void debug(const char* file, const int line, const char *s, const T *array, const int n) const {
        if(logLevel > LOG_LEVEL_DEBUG)
            return;
        logArray(DEBUG_STR, file + cwdOffset, line, s, array, n);
    }

    template<typename T>
    void info(const char* file, const int line, const char *s, const T *array, const int n) const {
        if(logLevel > LOG_LEVEL_INFO)
            return;
        logArray(INFO_STR, file + cwdOffset, line, s, array, n);
    }

    template<typename T>
    void warn(const char* file, const int line, const char *s, const T *array, const int n) const {
        if(logLevel > LOG_LEVEL_WARN)
            return;
        logArray(WARN_STR, file + cwdOffset, line, s, array, n);
    }

    template<typename T>
    void error(const char* file, const int line, const char *s, const T *array, const int n) const {
        logArray(ERROR_STR, file + cwdOffset, line, s, array, n);
    }

    /*template<typename T>
    void trace(const char* file, int line, const char *s, T *ptr) const {
        printf("%s %s:%d\t %s ", TRACE_STR, file + cwdOffset, line, s);
        cout << *ptr << '\n';
    }*/


private:
    Logger();
    static Logger* instance;
    static std::mutex mutex_;

    LogLevel logLevel;
    std::vector<Appender> appenders;
    size_t cwdOffset = 0;

    void log(const char *logLevel, const char* file, int line, const char *s, va_list args) const;

    const char* TRACE_STR = "TRACE: ";
    const char* DEBUG_STR = "DEBUG: ";
    const char* INFO_STR = "INFO:  ";
    const char* WARN_STR = "WARN:  ";
    const char* ERROR_STR = "ERROR: ";

    template<typename T>
    void logArray(const char* logLevelStr, const char* filePath, int line, const char* s, const T *data, const int n) const {

        for (auto& appender : appenders){
            appender.writeFormatString(logLevelStr, filePath, line);
            appender.write(s);

            if (n == 0){
                appender.write("[]\n");
                return;
            }

            *(appender.getStream()) << '[' << data[0];
            for (int i=1; i<n; i++){
                *(appender.getStream()) << ',' << data[i];
            }
            appender.write("]\n");
        }
    }
};

#endif //WINTER_LOGGER_H
