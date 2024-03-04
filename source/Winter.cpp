//
// Created by Alienson on 25.1.2024..
//

#include <iostream>
#include <thread>
#include "Winter.h"
#include "http/HttpRequest.h"

#include "log/Logger.h"
#include "http/HttpResponse.h"


void Winter::run() {
    init();
    mainLoop();
    cleanup();
}

void Winter::init() {
    wtLogTrace("Hello world");
    httpServerThread = std::thread([this]() { httpServer.start(); });
    //preProcessor->preProcess();
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
