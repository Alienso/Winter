//
// Created by Alienson on 20.9.2025.
//

#ifndef WINTER_LOGGY_H
#define WINTER_LOGGY_H

/**
 * For given set of Appenders, logs to each of them
 * Each appender is a stream with printing format
 */

#include <iostream>
#include <type_traits>
#include <mutex>
#include <vector>
#include "util/tsqueue.h"
#include <sstream>
#include <thread>
#include <queue>

#include "log/Appender.h"

#define wtLogTrace(format, ...) Loggy::getInstance()->trace(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogInfo(format, ...) Loggy::getInstance()->info(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogDebug(format, ...) Loggy::getInstance()->debug(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogWarn(format, ...) Loggy::getInstance()->warn(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define wtLogError(format, ...) Loggy::getInstance()->error(__FILE__, __LINE__, format, ##__VA_ARGS__)

enum LogLevel{
    LOG_LEVEL_TRACE,LOG_LEVEL_DEBUG,LOG_LEVEL_INFO,LOG_LEVEL_WARN,LOG_LEVEL_ERROR
};

struct LogCommand {
    LogCommand(const char *logLevelString, std::string outputString, const char *file, int line);
    const char* logLevelString;
    std::string outputString;
    std::string file;
    int line;
};

class Loggy {

public:
    Loggy(Loggy& other) = delete;
    ~Loggy();
    void operator=(const Loggy&) = delete;
    static Loggy* getInstance();

    void addAppender(std::ostream* stream);

    void trace(const char* file, int line, const char* formatStr) {
        if(logLevel > LOG_LEVEL_TRACE)
            return;
        log(TRACE_STR, file, line, formatStr);
    }
    template <typename First, typename... Rest>
    void trace(const char* file, int line, const char* formatStr, First&& first, Rest&&... rest) {
        if(logLevel > LOG_LEVEL_TRACE)
            return;
        log(TRACE_STR, file, line, formatStr, first, std::forward<Rest>(rest)...);
    }

    void debug(const char* file, int line, const char* formatStr) {
        if(logLevel > LOG_LEVEL_DEBUG)
            return;
        log(DEBUG_STR, file, line, formatStr);
    }
    template <typename First, typename... Rest>
    void debug(const char* file, int line, const char* formatStr, First&& first, Rest&&... rest) {
        if(logLevel > LOG_LEVEL_DEBUG)
            return;
        log(DEBUG_STR, file, line, formatStr, first, std::forward<Rest>(rest)...);
    }

    void info(const char* file, int line, const char* formatStr) {
        if(logLevel > LOG_LEVEL_INFO)
            return;
        log(INFO_STR, file, line, formatStr);
    }
    template <typename First, typename... Rest>
    void info(const char* file, int line, const char* formatStr, First&& first, Rest&&... rest) {
        if(logLevel > LOG_LEVEL_INFO)
            return;
        log(INFO_STR, file, line, formatStr, first, std::forward<Rest>(rest)...);
    }

    void warn(const char* file, int line, const char* formatStr) {
        if(logLevel > LOG_LEVEL_WARN)
            return;
        log(WARN_STR, file, line, formatStr);
    }
    template <typename First, typename... Rest>
    void warn(const char* file, int line, const char* formatStr, First&& first, Rest&&... rest) {
        if(logLevel > LOG_LEVEL_WARN)
            return;
        log(WARN_STR, file, line, formatStr, first, std::forward<Rest>(rest)...);
    }

    void error(const char* file, int line, const char* formatStr) {
        if(logLevel > LOG_LEVEL_ERROR)
            return;
        log(ERROR_STR, file, line, formatStr);
    }
    template <typename First, typename... Rest>
    void error(const char* file, int line, const char* formatStr, First&& first, Rest&&... rest) {
        if(logLevel > LOG_LEVEL_ERROR)
            return;
        log(ERROR_STR, file, line, formatStr, first, std::forward<Rest>(rest)...);
    }


    void log(const char* logLevelString, const char* file, int line, const char* formatString) {
        std::ostringstream outputStr;
        printToString(formatString, outputStr);
        outputStr << '\n';
        logQueue.emplace_back(logLevelString, outputStr.str(), file, line);
    }

    template <typename First, typename... Rest>
    void log(const char* logLevelString, const char* file, int line, const char* formatString, First&& first, Rest&&... rest) {
        std::ostringstream outputStr;
        printToString(formatString, outputStr, first, std::forward<Rest>(rest)...);
        outputStr << '\n';
        logQueue.emplace_back(logLevelString, outputStr.str(), file, line);
    }


private:
    Loggy();
    static Loggy* instance;
    static std::mutex mutex_;

    LogLevel logLevel;
    std::vector<Appender> appenders;
    tsqueue<LogCommand> logQueue;
    size_t cwdOffset = 0;

    std::thread loggingThread;

    const char* TRACE_STR = "TRACE: ";
    const char* DEBUG_STR = "DEBUG: ";
    const char* INFO_STR = "INFO:  ";
    const char* WARN_STR = "WARN:  ";
    const char* ERROR_STR = "ERROR: ";


    static void printToString(const char* formatString, std::ostringstream& stringStream){
        for (int i = 0; formatString[i] != '\0'; i++) {
            stringStream << formatString[i];
        }
    }

    template <typename First, typename... Rest>
    void printToString(const char* formatString, std::ostringstream& stringStream, First&& arg, Rest&&... rest) const {
        for (size_t i = 0; formatString[i] != '\0'; i++) {
            if (formatString[i] == '{' && formatString[i+1] == '}'){
                if constexpr (is_std_vector_v<std::decay_t<First>>) {
                    stringStream << "{ ";
                    for (size_t vec_index = 0; vec_index < arg.size(); vec_index++) {
                        stringStream << arg[vec_index];
                        if (vec_index + 1 < arg.size())
                            stringStream << ", ";
                    }
                    stringStream << " }";
                } else if constexpr (has_toString_v<First>) {
                    stringStream << arg.toString();
                } else {
                    stringStream << (arg);
                }
                printToString((&formatString[i + 2]), stringStream, std::forward<Rest>(rest)...);
                break;
            } else {
                stringStream << formatString[i];
            }
        }
    }



private:
    //Templates needed
    template <typename T>
    struct is_std_vector : std::false_type {};

    template <typename T, typename Alloc>
    struct is_std_vector<std::vector<T, Alloc>> : std::true_type {};

    template <typename T>
    static inline constexpr bool is_std_vector_v = is_std_vector<T>::value;

    template <typename, typename = void>
    struct has_toString : std::false_type {};

    template <typename T>
    struct has_toString<T, std::void_t<decltype(std::declval<T>().toString())>>
            : std::true_type {};

    template <typename T>
    static inline constexpr bool has_toString_v = has_toString<T>::value;

};






#endif //WINTER_LOGGY_H