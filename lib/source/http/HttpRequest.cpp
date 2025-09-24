//
// Created by Alienson on 25.1.2024.
//

#include "http/HttpRequest.h"
#include "http/HttpConnection.h"
#include "util/util.h"

HttpRequest::HttpRequest() {}

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