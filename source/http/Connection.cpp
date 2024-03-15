//
// Created by Alienson on 27.1.2024..
//

#include "Connection.h"
#include "asio/read.hpp"
#include "Logger.h"
#include <iostream>
#include <optional>
#include <thread>

Connection::Connection(asio::io_context &context, asio::ip::tcp::socket socket_, tsqueue<shared_ptr<HttpRequest>>& queue) :
     socket(std::move(socket_)), asioContext(context), requestQueue(queue) {}

void Connection::tryParseRequest(){
    shared_ptr<HttpRequest> httpRequest = HttpRequest::parseFromString(requestData);
    httpRequest->setConnection(this);
    requestQueue.push_back(shared_ptr<HttpRequest>(httpRequest));
    requestParsed = true;
}

void Connection::readDataFromSocket(){
    socket.async_read_some(asio::buffer(tempRequestBuffer, sizeof(tempRequestBuffer)),
            [this](error_code ec, size_t length){
        if (!ec){
            wtLogTrace("Reading data from socket");
            timeout = timeoutStep;
            if (!socket.is_open()){
                wtLogTrace("Socket closed!");
                tryParseRequest();
            }
            std::size_t oldSize = requestData.size();
            requestData.resize(requestData.size() + length);
            memcpy(&(requestData[oldSize]), tempRequestBuffer, length);
            readDataFromSocket();
        }else{
            wtLogError("Error occurred while reading data from socket", ec.message().data());
            tryParseRequest();
        }
    });

    while(timeout > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeoutStep));
        timeout-=timeoutStep;
    }
    if (!requestParsed)
        socket.cancel();
}

void Connection::createHttpRequest() {
    readDataFromSocket();
}

void Connection::respondToHttpRequest(const string& response){
    socket.write_some(asio::buffer(response.data(), response.size()));
    socket.close();
}