//
// Created by Alienson on 25.1.2024..
//

#include <thread>

#include "Winter.h"
#include "http/HttpRequest.h"
#include "Logger.h"
#include "http/HttpResponse.h"
#include "Component.h"


void Winter::run() {
    init();
    mainLoop();
    cleanup();
}

void Winter::init() {
    wtLogTrace("Hello world");
    httpServerThread = std::thread([this]() { httpServer.start(); });
    Reflect::initializeReflection();
    Component::initializeComponents();
}

void Winter::mainLoop() {
    while(!shouldStop){
        httpServer.getRequestQueue().waitForEvent();
        shared_ptr<HttpRequest> request = httpServer.getRequestQueue().pop_front();
        router->routeRequest(request);
    }
}

void Winter::cleanup() {
    wtLogInfo("Shutting down");
    if (httpServerThread.joinable())
        httpServerThread.join();
    httpServer.stop();
}
