//
// Created by Alienson on 27.1.2024..
//

#ifndef WINTER_CONFIGURATION_H
#define WINTER_CONFIGURATION_H

class Configuration {
public:
    static const int serverPort;
    static const int serverMaxConnections;
    static const int logLevel;

    Configuration() = default;
};


#endif //WINTER_CONFIGURATION_H
