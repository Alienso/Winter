//
// Created by Alienson on 1.2.2024..
//

#include "HttpResponse.h"
#include "Connection.h"
#include "Logger.h"

#include "util.h"

unordered_map<string,string> HttpResponse::baseResponseHeaders = {{"Connection","Closed"},
                                                                  {"Server", "WT/0.0.1"}};

HttpResponse::HttpResponse() {
    httpVersion = HttpVersion::V1_1;
    httpHeaders = baseResponseHeaders;
    httpHeaders["Date"] = wt::current_datetime();
}

HttpResponse::HttpResponse(HttpStatus *code) {
    httpCode = code;
    httpVersion = HttpVersion::V1_1;
    httpHeaders = baseResponseHeaders;
    httpHeaders["Date"] = wt::current_datetime();
}

HttpResponse::HttpResponse(Reflect *data, HttpStatus *code) {
    httpCode = code;
    httpVersion = HttpVersion::V1_1;
    httpHeaders = baseResponseHeaders;
    httpHeaders["Date"] = wt::current_datetime();
    httpHeaders["Content-Type"] = "application/json";
    httpBody = *(JsonSerializer::serialize(data)); //TODO this is a copy
}

string HttpResponse::writeRequestLine() const {
    return httpVersion->name + ' ' + to_string(httpCode->code) + ' ' + httpCode->name + '\n';
}

void HttpResponse::send() const {
    string response = toResponseString();
    connection->respondToHttpRequest(response);
    connection->close();
}

void HttpResponse::setConnection(wt::web::Connection * _connection) {
    this->connection = _connection;
}