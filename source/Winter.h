//
// Created by Alienson on 25.1.2024..
//

#ifndef WINTER_WINTER_H
#define WINTER_WINTER_H


#include "http/HttpServer.h"
#include "PreProcessor.h"
#include "Router.h"

class Winter {
public:
    Winter() = default;
    void run();

private:
    void init();
    void mainLoop();
    void cleanup();

    bool shouldStop = false;

    HttpServer httpServer;
    thread httpServerThread;
    PreProcessor* preProcessor = PreProcessor::getInstance();

    Router* router = Router::getInstance();
};


#endif //WINTER_WINTER_H
