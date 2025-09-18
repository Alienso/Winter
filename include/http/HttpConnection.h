//
// Created by Alienson on 27.1.2024.
//

#ifndef WINTER_HTTP_CONNECTION_H
#define WINTER_HTTP_CONNECTION_H

#include "asio/ip/tcp.hpp"
#include "http/HttpRequest.h"
#include "util/tsqueue.h"

class HttpConnection {

public:
    HttpConnection(asio::io_context& asioContext, asio::ip::tcp::socket socket, tsqueue<std::shared_ptr<HttpRequest>>& requestQueue);

    void createHttpRequest();
    void respondToHttpRequest(const std::string& response);

private:
    void readDataFromSocket();
    void tryParseRequest();

    asio::ip::tcp::socket socket;
    asio::io_context& asioContext;

    tsqueue<std::shared_ptr<HttpRequest>>& requestQueue;
    char tempRequestBuffer[20 * 1024];
    std::string requestData;

    const int timeoutStep = 100;
    int timeout = 100;
    bool requestParsed = false;

    std::string responseBuffer;
};


#endif //WINTER_HTTP_CONNECTION_H
