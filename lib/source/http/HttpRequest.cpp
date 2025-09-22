//
// Created by Alienson on 25.1.2024.
//

#include "http/HttpRequest.h"
#include "http/HttpConnection.h"
#include "log/Loggy.h"
#include "util/stringUtils.h"
#include "util/util.h"

HttpRequest::HttpRequest() {}

size_t getHeadersDelimiterIndex(const size_t endIndex, const std::string& data){
    for (size_t i = endIndex + 1; i<data.size(); i++){
        if (data[i] == '\r' && i+3 < data.size()){
            if (data[i+1] == '\n' && data[i+2] == '\r' && data[i+3] == '\n')
                return i+4;
        }else if (data[i] == '\n' && i+1 < data.size()){
            if (data[i+1] == '\n')
                return i+2;
        }
    }
    return std::string::npos;
}

std::shared_ptr<HttpRequest> HttpRequest::parseFromString(const std::string& data) {
    std::shared_ptr<HttpRequest> httpRequest{new HttpRequest()};

    size_t startIndex, endIndex;
    endIndex = data.find('\n');
    std::string line(data.data(), endIndex + 1); //sizeof \n

    startIndex = endIndex + 1;
    endIndex = getHeadersDelimiterIndex(endIndex, data);

    if (endIndex == std::string::npos)
        endIndex = data.length();
    std::string headers = data.substr(startIndex, endIndex - startIndex + 2);

    startIndex = endIndex;
    endIndex = data.length();
    if (endIndex == std::string::npos)
        endIndex = data.length();
    std::string_view body(&(data[startIndex]), endIndex - startIndex);

    parseRequestLine(*httpRequest, line);
    parseRequestHeaders(*httpRequest, headers);
    parseRequestBody(*httpRequest, body);

    wtLogTrace("Successfully parsed http request!");
    return httpRequest;
}

void HttpRequest::parseRequestLine(HttpRequest &request, const std::string &line) {
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

void HttpRequest::parseQueryParams(HttpRequest &request, const std::string &requestLine, const size_t start, const size_t end) {
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

void HttpRequest::parseRequestHeaders(HttpRequest &request, const std::string &headers) {
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

void HttpRequest::parseRequestBody(HttpRequest &request, std::string_view body) {
    request.requestBody = body.substr(0, body.size());
}

void HttpRequest::setConnection(HttpConnection* _connection) {
    this->connection = _connection;
}

HttpConnection *HttpRequest::getConnection() const {
    return connection;
}

const URI &HttpRequest::getUri() const {
    return uri;
}

HttpMethod *HttpRequest::getMethod() const {
    return method;
}

const std::string &HttpRequest::getRequestBody() const {
    return requestBody;
}

const std::unordered_map<std::string, std::string> &HttpRequest::getRequestHeaders() {
    return requestHeaders;
}

HttpVersion *HttpRequest::getHttpVersion() const {
    return httpVersion;
}

const std::unordered_map<std::string, std::string> &HttpRequest::getQueryParameters() {
    return queryParameters;
}