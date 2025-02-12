//
// Created by Alienson on 25.1.2024..
//

#ifndef WINTER_HTTPSERVER_H
#define WINTER_HTTPSERVER_H

#include "HttpRequest.h"
#include "../util/tsqueue.h"
#include "Connection.h"

#include <thread>
#include <asio/ip/tcp.hpp>
#include <condition_variable>

class HttpServer {
public:
    HttpServer();
    void listen();
    void start();
    void stop();
    void update();
    void waitForEvent();

    [[nodiscard]] tsqueue<shared_ptr<HttpRequest>> &getRequestQueue(){
        return requestQueue;
    }

    const uint32_t port;
    const uint32_t maxConnections;
    bool shouldStop = false;

    std::thread asioThread;
    asio::io_context asioContext;
    asio::ip::tcp::acceptor asioAcceptor;

    mutex muxBlocking;
    condition_variable cvBlocking;

    tsqueue<shared_ptr<HttpRequest>> requestQueue;
};


#endif //WINTER_HTTPSERVER_H
