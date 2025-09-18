//
// Created by Alienson on 25.1.2024.
//

#include <iostream>
#include <thread>
#include "HttpServer.h"
#include "core/Configuration.h"
#include "log/Logger.h"

HttpServer::HttpServer() :
    port(Configuration::serverPort),
    maxConnections(Configuration::serverMaxConnections),
    asioAcceptor(asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

void HttpServer::listen() {
    asioAcceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket){
        if (!ec){
            wtLogTrace("New Connection: %s", socket.remote_endpoint().address().to_string().data());
            auto* newConnection = new HttpConnection(asioContext, std::move(socket), requestQueue);
            newConnection->createHttpRequest();
        }else{
            wtLogError("New connection ERROR: %s", ec.message().data());
        }

        requestQueue.waitForEvent(maxConnections);
        listen();
    });
}

void HttpServer::update() {
    waitForEvent();
}

void HttpServer::waitForEvent() {
    while(requestQueue.empty()){
        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.wait(ul);
    }
}

void HttpServer::start(){
    listen();
    asioThread = std::thread([this]() { asioContext.run(); });
    wtLogInfo("Server Started!");

    while(!shouldStop){
        update();
    }
}

void HttpServer::stop(){
    asioContext.stop();
    if (asioThread.joinable())
        asioThread.join();
    wtLogInfo("Server Stopped!");
}
