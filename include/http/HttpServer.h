//
// Created by Alienson on 25.1.2024.
//

#ifndef WINTER_HTTPSERVER_H
#define WINTER_HTTPSERVER_H

#include "http/HttpRequest.h"
#include "util/tsqueue.h"
#include "HttpConnection.h"

#include <thread>
#include "asio/ip/tcp.hpp"
#include <condition_variable>

class HttpServer {
public:
    HttpServer();
    void listen();
    void start();
    void stop();
    void update();
    void waitForEvent();

    [[nodiscard]] tsqueue<std::shared_ptr<HttpRequest>> &getRequestQueue(){
        return requestQueue;
    }

    const uint32_t port;
    const uint32_t maxConnections;
    bool shouldStop = false;

    std::thread asioThread;
    asio::io_context asioContext;
    asio::ip::tcp::acceptor asioAcceptor;

    std::mutex muxBlocking;
    std::condition_variable cvBlocking;

    tsqueue<std::shared_ptr<HttpRequest>> requestQueue;
};


#endif //WINTER_HTTPSERVER_H
