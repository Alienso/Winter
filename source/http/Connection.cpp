//
// Created by Alienson on 27.1.2024..
//

#include "Connection.h"
#include "asio/read.hpp"
#include "../log/Logger.h"
#include <iostream>
#include <optional>
#include <thread>

Connection::Connection(asio::io_context &context, asio::ip::tcp::socket socket_, tsqueue<shared_ptr<HttpRequest>>& queue) :
     socket(std::move(socket_)), asioContext(context), requestQueue(queue) {

}

void Connection::tryParseRequest(){
    optional<HttpRequest> requestOptional = HttpRequest::parseFromString(requestData);
    if (requestOptional.has_value()){
        requestOptional.value().setConnection(this);
        requestQueue.push_back(make_shared<HttpRequest>(requestOptional.value()));
    }
    requestParsed = true;
}

void Connection::getHttpRequestData(){
    socket.async_read_some(asio::buffer(tempRequestBuffer, sizeof(tempRequestBuffer)),
            [this](error_code ec, size_t length){
        if (!ec){
            wtLogTrace("Reading data from socket");
            timeout = timeoutStep;
            if (!socket.is_open()){
                wtLogTrace("Socket closed!");
                //TODO drop connection from queue
                tryParseRequest();
            }
            std::size_t oldSize = requestData.size();
            requestData.resize(requestData.size() + length);
            memcpy(&(requestData[oldSize]), tempRequestBuffer, length);
            getHttpRequestData();
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
    getHttpRequestData();
}

void Connection::respondToHttpRequest(string& response){

    socket.write_some(asio::buffer(response.data(), response.size()));
    socket.close();

    //TODO remove connection from queue;
    /*socket.async_write_some(asio::buffer(tempRequestBuffer, sizeof(tempRequestBuffer)),
                            [this](error_code ec, size_t bytes_written){
        if (!ec){

        }else{

        }
    });*/
}