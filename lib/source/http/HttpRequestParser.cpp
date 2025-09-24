//
// Created by Alienson on 24.9.2025.
//

#include "http/HttpRequestParser.h"
#include "log/Loggy.h"
#include "HttpHeader.h"
#include "util.h"
#include "stringUtils.h"

#include <mutex>

HttpRequestParser::HttpRequestParser(std::mutex &mutex) : processDataMut(mutex){

}

void HttpRequestParser::parseReceivedData(size_t newDataSize) {
    std::scoped_lock lock(processDataMut);

    int i = 0;
    if (!readRequestLine){
        for(; tempRequestBuffer[i] != '\0'; i++){
            if (tempRequestBuffer[i] == '\n'){
                std::string requestLine { requestData.substr(0, requestData.size() - newDataSize) + std::string(tempRequestBuffer, i + 1) };
                readRequestLine = true;
                headersStartIndex = requestData.size() - newDataSize + i + 1;
                parseRequestLine(*httpRequest, requestLine);
                i++;
                break;
            }
        }
    }
    if (!readHeaders && readRequestLine){
        if (headersStartIndex == -1){
            wtLogError("Reading request headers, while request line not read!");
            status.parsingFailed = true;
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
        parseRequestHeaders(*httpRequest, headersString);
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

    return;
}

void HttpRequestParser::handleBodyWithoutLimit(int i) {

    for(;tempRequestBuffer[i] != '\0';i++){
        if (tempRequestBuffer[i] == '{') bodyBracketCounter++;
        if (tempRequestBuffer[i] == '}') bodyBracketCounter--;
    }

    if (tempRequestBuffer[i] == '\0') {
        if (bodyBracketCounter == 0) {
            std::string bodyString { requestData.substr(headersEndIndex + 1) };
            parseRequestBody(*httpRequest, bodyString);
            status.requestParsed = true;
            return;
        } else {
            wtLogError("Malformed body detected! body: {}", requestData);
            status.parsingFailed = true;
            return;
        }
    } else {
        wtLogError("Malformed body detected! body: {}", requestData);
        status.parsingFailed = true;
        return;
    }
}

void HttpRequestParser::handleBodyWithLimit(int i) {

    for(; bodyBytesRead < (size_t)bodySize; i++, bodyBytesRead++) {
        if (tempRequestBuffer[i] == '{') bodyBracketCounter++;
        if (tempRequestBuffer[i] == '}') bodyBracketCounter--;
    }

    if (tempRequestBuffer[i] == '\0') {
        if (bodyBracketCounter == 0) {
            std::string bodyString { requestData.substr(headersEndIndex + 1) };
            parseRequestBody(*httpRequest, bodyString);
            status.requestParsed = true;
            return;
        } else {
            wtLogError("Malformed body detected! body: {}", requestData);
            status.parsingFailed = true;
            return;
        }
    } else {
        wtLogError("Malformed body detected! body: {}", requestData);
        status.parsingFailed = true;
        return;
    }

}

void HttpRequestParser::parseRequestLine(HttpRequest &request, const std::string &line) {
    wtLogTrace("Request Line: {}", line);

    size_t startIndex, endIndex;

    endIndex = line.find(' ');
    std::string_view method(line.data(), endIndex);
    request.method = HttpMethod::fromString(method.data());

    startIndex = endIndex + 1;
    size_t queryParamsStart = 0;
    endIndex++;
    for (; endIndex < line.length(); endIndex++){
        if (line[endIndex] == '?') {
            queryParamsStart = endIndex;
        }else if (line[endIndex] == ' ')
            break;
    }
    if (queryParamsStart == 0){
        queryParamsStart = endIndex;
    }

    std::string_view endpoint(&(line[startIndex]), queryParamsStart - startIndex);
    URI newUri(wt::urlDecode(endpoint));
    request.uri = newUri;

    if (queryParamsStart != endIndex){
        parseQueryParams(request, line, queryParamsStart + 1, endIndex - 1);
    }

    startIndex = endIndex + 1;
    endIndex = line.find('\n', endIndex + 1); //sizeof('\n')
    std::string_view version(&(line[startIndex]), endIndex - startIndex);
    version = StringUtils::rtrim(version);
    request.httpVersion = HttpVersion::fromString(version.data());
}

void HttpRequestParser::parseQueryParams(HttpRequest &request, const std::string &requestLine, const size_t start, const size_t end) {
    std::string requestParamValue, requestParamName;
    size_t startIndex = start,endIndex = 0;
    size_t i;
    while (true){
        //find indexes of requestParamName
        for(i = startIndex; i<end; i++){
            if (requestLine[i] == '=') {
                endIndex = i;
                break;
            }
        }
        //Cannot find another line of x:y
        if (i >= end){
            break;
        }

        requestParamName = requestLine.substr(startIndex,endIndex - startIndex);
        //find index of requestParamValue
        startIndex = endIndex + 1;
        for(i = startIndex; i<end; i++){
            if (requestLine[i] == '&') {
                endIndex = i - 1;
                break;
            }
        }

        if (startIndex > endIndex){
            endIndex = i;
        }

        requestParamValue = requestLine.substr(startIndex, endIndex - startIndex + 1);
        startIndex = endIndex + 2;
        request.queryParameters[requestParamName] = requestParamValue;
    }
}

void HttpRequestParser::parseRequestHeaders(HttpRequest &request, const std::string &headers) {
    std::string headerValue, headerName;
    size_t startIndex = 0,endIndex = 0;
    size_t i;
    while(true){
        //find indexes of headerName
        for(i = startIndex; i<headers.size(); i++){
            if (headers[i] == ':') {
                endIndex = i;
                break;
            }
        }
        //Cannot find another line of x:y
        if (i == headers.size()){
            break;
        }
        headerName = headers.substr(startIndex,endIndex - startIndex);
        //find index of headerValue
        startIndex = endIndex + 1;
        for(i = startIndex; i<headers.size(); i++){
            if (headers[i] == '\n') {
                endIndex = i;
                break;
            }
        }
        if (headers[i - 1] == '\r')
            headerValue = headers.substr(startIndex, endIndex - startIndex - 1);
        else
            headerValue = headers.substr(startIndex, endIndex - startIndex);

        startIndex = endIndex + 1;
        request.requestHeaders[headerName] = StringUtils::trim(headerValue);
    }
}

void HttpRequestParser::parseRequestBody(HttpRequest &request, std::string_view body) {
    request.requestBody = body.substr(0, body.size());
}

void HttpRequestParser::updateData(size_t length) {
    tempRequestBuffer[length] = '\0';
    requestData.append((const char *)tempRequestBuffer);
}

const HttpRequestParserProcessStatus& HttpRequestParser::getStatus() const {
    return status;
}

std::shared_ptr<HttpRequest>& HttpRequestParser::getHttpRequest() {
    return httpRequest;
}
