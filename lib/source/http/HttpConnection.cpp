//
// Created by Alienson on 27.1.2024.
//

#include "http/HttpConnection.h"
#include "asio/read.hpp"
#include "log/Loggy.h"
#include "core/Configuration.h"
#include "http/HttpHeader.h"
#include "HttpResponse.h"

#include <iostream>
#include <optional>
#include <thread>

HttpConnection::HttpConnection(asio::io_context &context, asio::ip::tcp::socket socket_, tsqueue<std::shared_ptr<HttpRequest>>& queue) :
        socket(std::move(socket_)), asioContext(context), requestQueue(queue), httpRequestParser(processDataMut) {

}


void HttpConnection::readDataFromSocket(){
    wtLogTrace("Reading data from socket");

    socket.async_read_some(asio::buffer(httpRequestParser.tempRequestBuffer, sizeof(httpRequestParser.tempRequestBuffer)),
            [this](asio::error_code errorCode, size_t length){
        if (!errorCode){
            std::scoped_lock lock(processDataMut);
            wtLogTrace("Reading data from socket");
            httpRequestParser.updateData(length);
        }else{
            wtLogError("Error occurred while reading data from socket {}", errorCode.message().data());
        }
        httpRequestParser.parseReceivedData(length);
    });

    asio::steady_timer timer(asioContext);
    timer.expires_after(std::chrono::milliseconds(timeoutAfter));

    timer.async_wait([&](const asio::error_code& error) {
        if (!error) {
            std::scoped_lock lock(processDataMut);
            if (!httpRequestParser.getStatus().requestParsed){
                wtLogError("ReadTimeout occurred!");
                socket.cancel();
                timedOut = true;
            }
        }
    });

    while(!httpRequestParser.getStatus().requestParsed && !httpRequestParser.getStatus().parsingFailed && !timedOut){
        asioContext.run_one();
    }
    if (!timedOut) timer.cancel();

    if (httpRequestParser.getStatus().requestParsed){
        httpRequestParser.getHttpRequest()->setConnection(this);
        requestQueue.push_back(httpRequestParser.getHttpRequest());
    } else {
        HttpCode* code = HttpCode::INTERNAL_SERVER_ERROR;
        if (httpRequestParser.getStatus().parsingFailed) code = HttpCode::BAD_REQUEST;
        else if (timedOut) code = HttpCode::REQUEST_TIMEOUT;

        HttpResponse *response = new HttpResponse(code);
        response->setConnection(this);
        response->send();
        delete response;
    }
}

void HttpConnection::createHttpRequest() {
    readDataFromSocket();
}

void HttpConnection::respondToHttpRequest(const std::string& response){
    socket.write_some(asio::buffer(response.data(), response.size()));
    socket.close();
}
