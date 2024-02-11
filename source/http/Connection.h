//
// Created by Alienson on 27.1.2024..
//

#ifndef WINTER_CONNECTION_H
#define WINTER_CONNECTION_H

#include <asio/ip/tcp.hpp>
#include "HttpRequest.h"
#include "../util/tsqueue.h"

class Connection {

public:
    Connection(asio::io_context& asioContext, asio::ip::tcp::socket socket, tsqueue<shared_ptr<HttpRequest>>& requestQueue);

    void createHttpRequest();
    void respondToHttpRequest(string& response);

private:
    void getHttpRequestData();
    void tryParseRequest();

    asio::ip::tcp::socket socket;
    asio::io_context& asioContext;

    tsqueue<shared_ptr<HttpRequest>>& requestQueue;
    char tempRequestBuffer[20 * 1024];
    string requestData;

    int timeoutStep = 100;
    int timeout = 100;
    bool requestParsed = false;

    string responseBuffer;
};


#endif //WINTER_CONNECTION_H
