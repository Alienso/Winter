//
// Created by Alienson on 27.1.2024..
//

#ifndef WINTER_HTTP_CONNECTION_H
#define WINTER_HTTP_CONNECTION_H

#include <asio/ip/tcp.hpp>
#include "../util/tsqueue.h"

class HttpRequest;

namespace wt::web {

    class Connection {

    public:
        Connection(asio::ip::tcp::socket socket);

        Connection(asio::ip::tcp::socket socket, tsqueue<shared_ptr<HttpRequest>> *requestQueue);

        void createHttpRequest();

        void respondToHttpRequest(const string &response);

        string &getDataFromSocket();

        void close();

    private:

        void readDataFromSocket();

        void tryParseRequest();

        asio::ip::tcp::socket socket;

        tsqueue<shared_ptr<HttpRequest>> *requestQueue;
        char tempRequestBuffer[20 * 1024];
        string requestData;

        const int timeoutStep = 100;
        int timeout = 100;
        bool requestParsed = false;

        string responseBuffer;
    };
}


#endif //WINTER_HTTP_CONNECTION_H
