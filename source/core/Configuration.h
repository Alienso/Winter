//
// Created by Alienson on 27.1.2024.
//

#ifndef WINTER_CONFIGURATION_H
#define WINTER_CONFIGURATION_H

#include "Logger.h"

class Configuration {
public:
    static const inline int serverPort = 8080;
    static const inline int serverMaxConnections = 20;
    static const inline int logLevel = LOG_LEVEL_TRACE;

    static const inline int dbConnectionPoolSize = 20;
    static const inline std::string dbConnectionString = "postgresql://postgres:abcdef@localhost:5432/postgres";

    Configuration() = default;
};


#endif //WINTER_CONFIGURATION_H
