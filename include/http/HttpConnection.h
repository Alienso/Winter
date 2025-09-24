//
// Created by Alienson on 27.1.2024.
//

#ifndef WINTER_HTTP_CONNECTION_H
#define WINTER_HTTP_CONNECTION_H

#include "asio/ip/tcp.hpp"
#include "http/HttpRequest.h"
#include "util/tsqueue.h"

#include "Configuration.h"

class HttpConnection {

public:
    HttpConnection(asio::io_context& asioContext, asio::ip::tcp::socket socket, tsqueue<std::shared_ptr<HttpRequest>>& requestQueue);

    void createHttpRequest();
    void respondToHttpRequest(const std::string& response);

private:
    void readDataFromSocket();
    void parseReceivedData(size_t newDataSize);

    void setParsingFailed();
    void setTimedOut();

    void handleBodyWithoutLimit(int i);
    void handleBodyWithLimit(int i);

    asio::ip::tcp::socket socket;
    asio::io_context& asioContext;
    tsqueue<std::shared_ptr<HttpRequest>>& requestQueue;
    std::shared_ptr<HttpRequest> httpRequest{new HttpRequest()};

    static constexpr int timeoutAfter = Configuration::httpRequestReadTimeout;
    static constexpr int tempRequestBufferSize = 20 * 1024;

    char tempRequestBuffer[tempRequestBufferSize] = {};

    std::string requestData;
    bool requestParsed = false;
    bool parsingFailed = false;
    bool timedOut = false;

    int headersStartIndex = -1;
    int headersEndIndex = -1;
    int bodySize = -1;
    size_t bodyBytesRead = 0;
    size_t bodyBracketCounter = 0;

    bool readRequestLine = false;
    bool readHeaders = false;
    bool processedHeaders = false;

    std::mutex processDataMut;

};


#endif //WINTER_HTTP_CONNECTION_H
