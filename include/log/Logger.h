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


//Deprecated
enum LogLevelOld{
    LOG_LEVEL_TRACE_OLD,LOG_LEVEL_DEBUG_OLD,LOG_LEVEL_INFO_OLD,LOG_LEVEL_WARN_OLD,LOG_LEVEL_ERROR_OLD
};

//Deprecated
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
        /*if(logLevel > LOG_LEVEL_TRACE_OLD)
            return;
        logArray(TRACE_STR, file + cwdOffset, line, s, vec.data(), vec.size());*/
    }

    template<typename T>
    void debug(const char* file, const int line, const char *s, const std::vector<T> vec) const {
        /*if(logLevel > LOG_LEVEL_DEBUG_OLD)
            return;
        logArray(DEBUG_STR, file + cwdOffset, line, s, vec.data(), vec.size());*/
    }

    template<typename T>
    void info(const char* file, const int line, const char *s, const std::vector<T> vec) const {
        /*if(logLevel > LOG_LEVEL_INFO_OLD)
            return;
        logArray(INFO_STR, file + cwdOffset, line, s, vec.data(), vec.size());*/
    }

    template<typename T>
    void warn(const char* file, int line, const char *s, const std::vector<T> vec) const {
        /*if(logLevel > LOG_LEVEL_WARN_OLD)
            return;
        logArray(WARN_STR, file + cwdOffset, line, s, vec.data(), vec.size());*/
    }

    template<typename T>
    void error(const char* file, const int line, const char *s, const std::vector<T> vec) const {
        logArray(ERROR_STR, file + cwdOffset, line, s, vec.data(), vec.size());
    }

    template<typename T>
    void trace(const char* file, const int line, const char *s, const T *array, const int n) const {
        if(logLevel > LOG_LEVEL_TRACE_OLD)
            return;
        logArray(TRACE_STR, file + cwdOffset, line, s, array, n);
    }

    template<typename T>
    void debug(const char* file, const int line, const char *s, const T *array, const int n) const {
        if(logLevel > LOG_LEVEL_DEBUG_OLD)
            return;
        logArray(DEBUG_STR, file + cwdOffset, line, s, array, n);
    }

    template<typename T>
    void info(const char* file, const int line, const char *s, const T *array, const int n) const {
        if(logLevel > LOG_LEVEL_INFO_OLD)
            return;
        logArray(INFO_STR, file + cwdOffset, line, s, array, n);
    }

    template<typename T>
    void warn(const char* file, const int line, const char *s, const T *array, const int n) const {
        if(logLevel > LOG_LEVEL_WARN_OLD)
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

    LogLevelOld logLevel;
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
