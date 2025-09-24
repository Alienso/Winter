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
     socket(std::move(socket_)), asioContext(context), requestQueue(queue) {}

void HttpConnection::parseReceivedData(size_t newDataSize) {
    std::scoped_lock lock(processDataMut);

    int i = 0;
    if (!readRequestLine){
        for(; tempRequestBuffer[i] != '\0'; i++){
            if (tempRequestBuffer[i] == '\n'){
                std::string requestLine { requestData.substr(0, requestData.size() - newDataSize) + std::string(tempRequestBuffer, i + 1) };
                readRequestLine = true;
                headersStartIndex = requestData.size() - newDataSize + i + 1;
                HttpRequest::parseRequestLine(*httpRequest, requestLine);
                i++;
                break;
            }
        }
    }
    if (!readHeaders && readRequestLine){
        if (headersStartIndex == -1){
            wtLogError("Reading request headers, while request line not read!");
            setParsingFailed();
            return;
        }
        for(; tempRequestBuffer[i] != '\0'; i++){
            // \r\n\r\n case
            if (tempRequestBuffer[i] == '\n'){
                if (i>=3){
                    if (tempRequestBuffer[i-1] == '\r' && tempRequestBuffer[i-2] == '\n' && tempRequestBuffer[i-3] == '\r'){
                        //headersString = requestData.substr(headersStartIndex) + std::string(tempRequestBuffer, i);
                        headersEndIndex = requestData.size() - newDataSize + i;
                        readHeaders = true;
                        i++;
                        break;
                    }
                }else{
                    if (requestData.size() >= 3){
                      //data is split between 2 buffers. read data from requestData buffer
                      size_t offset = requestData.size();
                      if ((i == 0 && (requestData[offset-1] == '\r' && requestData[offset-2] == '\n' && requestData[offset-3] == '\r'))
                      || (i == 1 && (tempRequestBuffer[i-1] == '\r' && requestData[offset-1] == '\n' && requestData[offset-2] == '\r'))
                      || (i == 2 && (tempRequestBuffer[i-1] == '\r' && tempRequestBuffer[i-2] == '\n' && requestData[offset-1] == '\r'))){
                          headersEndIndex = requestData.size() - newDataSize + i;
                          readHeaders = true;
                          i++;
                          break;
                      }
                    }
                }
            }
            // \n\n case
            if (tempRequestBuffer[i] == '\n'){
                if (i>0){
                    if (tempRequestBuffer[i-1] == '\n'){
                        headersEndIndex = requestData.size() - newDataSize + i;
                        readHeaders = true;
                        i++;
                        break;
                    }
                } else {
                    //data is split between 2 buffers. read data from requestData buffer
                    if (requestData[requestData.size() - 1] == '\n'){
                        headersEndIndex = requestData.size() - newDataSize + i;
                        readHeaders = true;
                        i++;
                        break;
                    }
                }
            }
        }
    }

    if (readHeaders && !processedHeaders){
        std::string headersString { requestData.substr(headersStartIndex, headersEndIndex - headersStartIndex + 1) };
        HttpRequest::parseRequestHeaders(*httpRequest, headersString);
        if ( auto it = httpRequest->getRequestHeaders().find(HttpHeader::CONTENT_LENGTH); it != httpRequest->getRequestHeaders().end()){
            bodySize = std::stoi(it->second);
        }
        processedHeaders = true;
    }

    if (processedHeaders){
        //Read body
        if (tempRequestBuffer[i] != '\0' && bodySize == -1){
            wtLogTrace("Content-Length not found in headers!");
        }

        if (bodySize == -1) {
            handleBodyWithoutLimit(i);
        } else {
            handleBodyWithLimit(i);
        }
    }

}

void HttpConnection::handleBodyWithoutLimit(int i) {

    for(;tempRequestBuffer[i] != '\0';i++){
        if (tempRequestBuffer[i] == '{') bodyBracketCounter++;
        if (tempRequestBuffer[i] == '}') bodyBracketCounter--;
    }

    if (tempRequestBuffer[i] == '\0') {
        if (bodyBracketCounter == 0) {
            std::string bodyString { requestData.substr(headersEndIndex + 1) };
            HttpRequest::parseRequestBody(*httpRequest, bodyString);
            requestParsed = true;
            return;
        } else {
            wtLogError("Malformed body detected! body: {}", requestData);
            setParsingFailed();
            return;
        }
    } else {
        wtLogError("Malformed body detected! body: {}", requestData);
        setParsingFailed();
        return;
    }
}

void HttpConnection::handleBodyWithLimit(int i) {

    for(; bodyBytesRead < (size_t)bodySize; i++, bodyBytesRead++) {
        if (tempRequestBuffer[i] == '{') bodyBracketCounter++;
        if (tempRequestBuffer[i] == '}') bodyBracketCounter--;
    }

    if (tempRequestBuffer[i] == '\0') {
        if (bodyBracketCounter == 0) {
            std::string bodyString { requestData.substr(headersEndIndex + 1) };
            HttpRequest::parseRequestBody(*httpRequest, bodyString);
            requestParsed = true;
            return;
        } else {
            wtLogError("Malformed body detected! body: {}", requestData);
            setParsingFailed();
            return;
        }
    } else {
        wtLogError("Malformed body detected! body: {}", requestData);
        setParsingFailed();
        return;
    }

}

void HttpConnection::readDataFromSocket(){
    wtLogTrace("Reading data from socket");

    //TODO refactor this so parsing is done in separate file/class.
    socket.async_read_some(asio::buffer(tempRequestBuffer, sizeof(tempRequestBuffer)),
            [this](asio::error_code errorCode, size_t length){
        if (!errorCode){
            std::scoped_lock lock(processDataMut);
            wtLogTrace("Reading data from socket");
            tempRequestBuffer[length] = '\0';
            requestData.append((const char *)tempRequestBuffer);
        }else{
            wtLogError("Error occurred while reading data from socket {}", errorCode.message().data());
        }
        parseReceivedData(length);
    });

    asio::steady_timer timer(asioContext);
    timer.expires_after(std::chrono::milliseconds(timeoutAfter));

    timer.async_wait([&](const asio::error_code& error) {
        if (!error) {
            std::scoped_lock lock(processDataMut);
            if (!requestParsed){
                wtLogError("ReadTimeout occurred!");
                setTimedOut();
            }
        }
    });

    while(!requestParsed && !parsingFailed && !timedOut){
        asioContext.run_one();
    }
    if (!timedOut) timer.cancel();

    if (requestParsed){
        httpRequest->setConnection(this);
        requestQueue.push_back(std::shared_ptr<HttpRequest>(httpRequest));
    } else {
        HttpCode* code = HttpCode::INTERNAL_SERVER_ERROR;
        if (parsingFailed) code = HttpCode::BAD_REQUEST;
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

void HttpConnection::setParsingFailed() {
    socket.cancel();
    parsingFailed = true;
}

void HttpConnection::setTimedOut() {
    socket.cancel();
    timedOut = true;
}
