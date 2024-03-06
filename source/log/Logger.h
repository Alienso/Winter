//
// Created by Alienson on 27.1.2024..
//

#ifndef WINTER_LOGGER_H
#define WINTER_LOGGER_H

#include <string>
#include <vector>
#include <mutex>

#include "Appender.h"

#define wtLogTrace(format, ...) Logger::getInstance()->trace(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogInfo(format, ...) Logger::getInstance()->info(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogDebug(format, ...) Logger::getInstance()->debug(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogWarn(format, ...) Logger::getInstance()->warn(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogError(format, ...) Logger::getInstance()->error(__FILE__, __LINE__, format, ##__VA_ARGS__)

using namespace std;

enum LogLevel{
    LOG_LEVEL_TRACE,LOG_LEVEL_DEBUG,LOG_LEVEL_INFO,LOG_LEVEL_WARN,LOG_LEVEL_ERROR
};

class Logger {

public:

    Logger(Logger& other) = delete;
    void operator=(const Logger&) = delete;
    static Logger* getInstance();

    void trace(const char* file, int line, const char* s, ...);
    void debug(const char* file, int line, const char* s, ...);
    void info(const char* file, int line, const char* s, ...);
    void warn(const char* file, int line, const char* s, ...);
    void error(const char* file, int line, const char* s, ...);

    template<typename T>
    void trace(const char* file, int line, const char *s, vector<T> vec) {
        if(logLevel > LOG_LEVEL_TRACE)
            return;
        cout << TRACE_STR << s;
        logArray(s, vec.data(), vec.size());
    }

    template<typename T>
    void debug(const char* file, int line, const char *s, vector<T> vec) {
        if(logLevel > LOG_LEVEL_DEBUG)
            return;
        cout << DEBUG_STR << s;
        logArray(s, vec.data(), vec.size());
    }

    template<typename T>
    void info(const char* file, int line, const char *s, vector<T> vec) {
        if(logLevel > LOG_LEVEL_INFO)
            return;
        cout << INFO_STR << s;
        logArray(s, vec.data(), vec.size());
    }

    template<typename T>
    void warn(const char* file, int line, const char *s, vector<T> vec) {
        if(logLevel > LOG_LEVEL_WARN)
            return;
        cout << WARN_STR << s;
        logArray(s, vec.data(), vec.size());
    }

    template<typename T>
    void error(const char* file, int line, const char *s, vector<T> vec) {
        cout << ERROR_STR << s;
        logArray(s, vec.data(), vec.size());
    }

    template<typename T>
    void trace(const char* file, int line, const char *s, T *array, int n) {
        if(logLevel > LOG_LEVEL_TRACE)
            return;
        cout << TRACE_STR << s;
        logArray(s, array, n);
    }

    template<typename T>
    void debug(const char* file, int line, const char *s, T *array, int n) {
        if(logLevel > LOG_LEVEL_DEBUG)
            return;
        cout << DEBUG_STR << s;
        logArray(s, array, n);
    }

    template<typename T>
    void info(const char* file, int line, const char *s, T *array, int n) {
        if(logLevel > LOG_LEVEL_INFO)
            return;
        cout << INFO_STR << s;
        logArray(s, array, n);
    }

    template<typename T>
    void warn(const char* file, int line, const char *s, T *array, int n) {
        if(logLevel > LOG_LEVEL_WARN)
            return;
        cout << WARN_STR << s;
        logArray(s, array, n);
    }

    template<typename T>
    void error(const char* file, int line, const char *s, T *array, int n) {
        cout << ERROR_STR << s;
        logArray(s, array, n);
    }


private:
    Logger();
    static Logger* instance;
    static mutex mutex_;

    LogLevel logLevel;
    vector<Appender> appenders;
    int cwdOffset = 0;

    void log(const char *logLevel, const char* file, int line, const char *s, va_list args);

    const char* TRACE_STR = "TRACE: ";
    const char* DEBUG_STR = "DEBUG: ";
    const char* INFO_STR = "INFO:  ";
    const char* WARN_STR = "WARN:  ";
    const char* ERROR_STR = "ERROR: ";

    template<typename T>
    void logArray(const char *s, T *data, int n) {

        if (n == 0){
            cout << "[]\n";
            return;
        }

        cout << '[' << data[0];
        for (int i=1; i<n; i++){
            cout << ',' << data[i];
        }
        cout << "]\n";
    }
};

#endif //WINTER_LOGGER_H
