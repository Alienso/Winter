//
// Created by Alienson on 27.1.2024..
//

#include "Connection.h"
#include "HttpRequest.h"
#include "asio/read.hpp"
#include "Logger.h"

#include <optional>
#include <thread>
#include <include/asio/streambuf.hpp>
#include <include/asio/read_until.hpp>


wt::web::Connection::Connection(asio::ip::tcp::socket socket_) : socket(std::move(socket_)) {}


wt::web::Connection::Connection(asio::ip::tcp::socket socket_, tsqueue<shared_ptr<HttpRequest>>* queue) :
     socket(std::move(socket_)), requestQueue(queue) {}

void wt::web::Connection::tryParseRequest(){
    shared_ptr<HttpRequest> httpRequest = HttpRequest::parseFromString(requestData);
    httpRequest->setConnection(this);
    requestQueue->push_back(shared_ptr<HttpRequest>(httpRequest));
    requestParsed = true;
}

//TODO
void wt::web::Connection::readDataFromSocket(){
    wtLogTrace("Reading data from socket");
    int bufferSize = sizeof(tempRequestBuffer);
    while (int read = socket.read_some(asio::buffer(tempRequestBuffer))){
        std::size_t oldSize = requestData.size();
        requestData.resize(requestData.size() + read);
        memcpy(&(requestData[oldSize]), tempRequestBuffer, read);
        if (read != bufferSize){
            //tryParseRequest();
            break;
        }
    }

    /*socket.async_read_some(asio::buffer(tempRequestBuffer, sizeof(tempRequestBuffer)),
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

    //On Linux callback if never invoked when this part of the code is present
    while(timeout > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeoutStep));
        timeout-=timeoutStep;
    }
    if (!requestParsed)
        socket.cancel();*/
}

void wt::web::Connection::createHttpRequest() {
    readDataFromSocket();
    tryParseRequest();
}

string& wt::web::Connection::getDataFromSocket(){
    readDataFromSocket();
    return requestData;
}

void wt::web::Connection::respondToHttpRequest(const string& response){
    size_t bytesWritten = 0;
    while(bytesWritten < response.size()) {
        const char* ptr = &(response[bytesWritten]);
        bytesWritten += socket.write_some(asio::buffer(ptr, response.size() - bytesWritten));
    }
}

void wt::web::Connection::close(){
    socket.close();
}