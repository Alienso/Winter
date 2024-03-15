//
// Created by Alienson on 25.1.2024..
//

#include "HttpRequest.h"
#include "Connection.h"
#include "Logger.h"
#include "stringUtils.h"

HttpRequest::HttpRequest() {}

shared_ptr<HttpRequest> HttpRequest::parseFromString(const string& data) {
    shared_ptr<HttpRequest> httpRequest{new HttpRequest()};

    size_t startIndex, endIndex;
    endIndex = data.find('\n');
    string line(data.data(), endIndex + 1); //sizeof \n

    startIndex = endIndex + 1;
    endIndex = data.find("\r\n\r\n", endIndex + 1); //TEMP FIX!
    if (endIndex == string::npos)
        endIndex = data.length();
    string headers = data.substr(startIndex, endIndex - startIndex + 2);

    startIndex = endIndex;
    endIndex = data.length();
    if (endIndex == string::npos)
        endIndex = data.length();
    string_view body(&(data[startIndex]), endIndex - startIndex);

    parseRequestLine(*httpRequest, line);
    parseRequestHeaders(*httpRequest, headers);
    parseRequestBody(*httpRequest, body);

    wtLogTrace("Successfully parsed http request!");
    return httpRequest;
}

void HttpRequest::parseRequestLine(HttpRequest &request, const string &line) {
    wtLogTrace("Request Line: %s", line.data());

    size_t startIndex, endIndex;

    endIndex = line.find(' ');
    string_view method(line.data(), endIndex);
    request.method = HttpMethod::fromString(method.data());

    startIndex = endIndex + 1;
    endIndex = line.find(' ', endIndex + 1); //sizeof(' ')
    string_view endpoint(&(line[startIndex]), endIndex - startIndex);
    URI newUri(endpoint);
    request.uri = newUri;

    startIndex = endIndex + 1;
    endIndex = line.find('\n', endIndex + 1); //sizeof('\n')
    string_view version(&(line[startIndex]), endIndex - startIndex);
    version = StringUtils::rtrim(version);
    request.httpVersion = HttpVersion::fromString(version.data());
}

void HttpRequest::parseRequestHeaders(HttpRequest &request, const string &headers) {
    string headerValue, headerName;
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
        request.requestHeaders[headerName] = headerValue;
    }
}

void HttpRequest::parseRequestBody(HttpRequest &request, string_view body) {
    request.requestBody = body.substr(0, body.size());
}

void HttpRequest::setConnection(Connection* _connection) {
    this->connection = _connection;
}

Connection *HttpRequest::getConnection() const {
    return connection;
}

const URI &HttpRequest::getUri() const {
    return uri;
}

HttpMethod *HttpRequest::getMethod() const {
    return method;
}

const string &HttpRequest::getRequestBody() const {
    return requestBody;
}
