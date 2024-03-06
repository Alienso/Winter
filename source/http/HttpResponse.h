//
// Created by Alienson on 1.2.2024..
//

#ifndef WINTER_HTTPRESPONSE_H
#define WINTER_HTTPRESPONSE_H


#include <memory>
#include "HttpRequest.h"
#include "httpConstants.h"
#include "../reflect/Reflect.h"
#include "../serialize/JsonSerializer.h"

using namespace std;

class HttpResponse {

public:
    HttpResponse();
    explicit HttpResponse(HttpCode* code);
    explicit HttpResponse(Reflect* data, HttpCode* code = HttpCode::OK);

    void send();
    shared_ptr<string> toResponseString();
    void setConnection(Connection* _connection);
    string& getBody();

    static HttpResponse generateResponse(const shared_ptr<HttpRequest>& sharedPtr);

private:
    HttpVersion* httpVersion;
    HttpCode* httpCode;
    unordered_map<string,string> responseHeaders;
    string responseBody;
    Connection* connection;
    JsonSerializer serializer;

    static unordered_map<string,string> baseResponseHeaders;

    string writeRequestLine();
    string writeRequestHeaders();
    string writeRequestBody();
};


#endif //WINTER_HTTPRESPONSE_H
