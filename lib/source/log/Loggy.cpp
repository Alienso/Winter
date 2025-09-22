//
// Created by Alienson on 20.9.2025.
//

#include <utility>

#include "log/Loggy.h"
#include "core/Configuration.h"

Loggy* Loggy::instance = nullptr;
std::mutex Loggy::mutex_;

LogCommand::LogCommand(const char *logLevelString, std::string outputString, const char *file, int line)
        : logLevelString(logLevelString), outputString(std::move(outputString)), file(file), line(line) {}

Loggy::Loggy() {
    logLevel = (LogLevel) Configuration::logLevel;
    appenders.emplace_back("", &std::cout);
    std::string cwd = __FILE__;
    size_t sourceIndex = cwd.find("Winter");
    if (sourceIndex == std::string::npos) {
        std::cout << "Error while initializing logger. Could not find source in path\n";
        sourceIndex = 0;
    }
    cwdOffset = sourceIndex + 7;

    loggingThread = std::thread([this]{
        while(true){
            //logQueue.waitForEvent();
            while(!logQueue.empty()){
                LogCommand command = logQueue.front();
                for (auto& appender : appenders){
                    appender.writeFormatString(command.logLevelString, command.file.c_str() + cwdOffset, command.line);
                    appender.write(command.outputString);
                }
                logQueue.pop();
            }
        }
    });

}

Loggy *Loggy::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr){
        instance = new Loggy();
    }
    return instance;
}

void Loggy::addAppender(std::ostream* stream) {
    appenders.emplace_back("", stream);
}
