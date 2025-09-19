//
// Created by Alienson on 25.1.2024.
//

#ifndef WINTER_WINTER_H
#define WINTER_WINTER_H

#include "../util/DbConnectionPool.h"
#include "../http/HttpServer.h"
#include "../core/Router.h"

class Winter {
public:
    Winter();
    void run();

private:
    void init();
    void mainLoop();
    void cleanup();

    static void signalHandler(int signal);
    static Winter* instance;
    bool shouldStop = false;

    HttpServer httpServer;
    std::thread httpServerThread;

    Router* router = Router::getInstance();
};


#endif //WINTER_WINTER_H
