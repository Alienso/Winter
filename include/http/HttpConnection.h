//
// Created by Alienson on 27.1.2024.
//

#ifndef WINTER_HTTP_CONNECTION_H
#define WINTER_HTTP_CONNECTION_H

#include "asio/ip/tcp.hpp"
#include "http/HttpRequest.h"
#include "util/tsqueue.h"

#include "Configuration.h"
#include "HttpRequestParser.h"

class HttpConnection {

public:
    HttpConnection(asio::io_context& asioContext, asio::ip::tcp::socket socket, tsqueue<std::shared_ptr<HttpRequest>>& requestQueue);

    void createHttpRequest();
    void respondToHttpRequest(const std::string& response);

private:
    void readDataFromSocket();

    asio::ip::tcp::socket socket;
    asio::io_context& asioContext;
    tsqueue<std::shared_ptr<HttpRequest>>& requestQueue;
    std::mutex processDataMut;
    HttpRequestParser httpRequestParser;

    static constexpr int timeoutAfter = Configuration::httpRequestReadTimeout;

    bool timedOut = false;

};


#endif //WINTER_HTTP_CONNECTION_H
