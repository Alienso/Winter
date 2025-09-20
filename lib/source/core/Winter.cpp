//
// Created by Alienson on 25.1.2024.
//

#include <thread>

#include "core/Winter.h"
#include "log/Logger.h"
#include "core/Component.h"

Winter* Winter::instance = nullptr;

void Winter::signalHandler(int signal) {
    wtLogInfo("Received signal %d", signal); //TODO check this
    instance->shouldStop = true;
}

Winter::Winter(){
    Winter::instance = this;
}

void Winter::run() {
    init();
    mainLoop();
    cleanup();
}

void Winter::init() {
    wtLogTrace("Hello world");
    signal(SIGINT, Winter::signalHandler);
    httpServerThread = std::thread([this]() { httpServer.start(); });
    Reflect::initializeReflection();
    Component::initializeComponents();
}

void Winter::mainLoop() {
    while(!shouldStop){
        httpServer.getRequestQueue().waitForEvent();
        std::shared_ptr<HttpRequest> request = httpServer.getRequestQueue().pop_front();
        router->routeRequest(request);
    }
}

void Winter::cleanup() {
    wtLogInfo("Shutting down");
    if (httpServerThread.joinable())
        httpServerThread.join();
    httpServer.stop();

    //delete router;
}
