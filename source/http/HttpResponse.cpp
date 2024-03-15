//
// Created by Alienson on 1.2.2024..
//

#include "HttpResponse.h"
#include "Connection.h"
#include "Logger.h"

#include "util.h"

unordered_map<string,string> HttpResponse::baseResponseHeaders = {{"Connection","Closed"},
                                                                  {"Server", "WT/0.0.1"}};

HttpResponse::HttpResponse() : httpVersion(HttpVersion::V1_1) {
    responseHeaders = baseResponseHeaders;
    responseHeaders["Date"] = wt::current_datetime();
}

HttpResponse::HttpResponse(HttpCode *code) : httpVersion(HttpVersion::V1_1), httpCode(code) {
    responseHeaders = baseResponseHeaders;
    responseHeaders["Date"] = wt::current_datetime();
}

HttpResponse::HttpResponse(Reflect *data, HttpCode *code) : httpVersion(HttpVersion::V1_1), httpCode(code) {
    responseHeaders = baseResponseHeaders;
    responseHeaders["Date"] = wt::current_datetime();
    responseHeaders["Content-Type"] = "application/json";
    responseBody = *(serializer.serialize(data)); //TODO this is a copy
}

string HttpResponse::toResponseString() const{

    string requestLine = writeRequestLine();
    string headers = writeRequestHeaders();
    string body = writeRequestBody();

    return requestLine + headers + body + "\n";
}

void HttpResponse::send() const {
    string response = toResponseString();
    connection->respondToHttpRequest(response);
}

string HttpResponse::writeRequestLine() const {
    return httpVersion->name + ' ' + to_string(httpCode->code) + ' ' + httpCode->name + '\n';
}

string HttpResponse::writeRequestHeaders() const {
    string result;
    for (const auto& x : responseHeaders){

        for (char c: x.first)
            result.push_back(c);

        result.push_back(':');
        result.push_back(' ');

        for (char c: x.second)
            result.push_back(c);

        result.push_back('\n');
    }

    result.push_back('\n');
    result.push_back('\0');
    return result;
}

string HttpResponse::writeRequestBody() const {
    return responseBody;
}

const string &HttpResponse::getBody() const {
    return responseBody;
}

void HttpResponse::setConnection(Connection * _connection) {
    this->connection = _connection;
}
