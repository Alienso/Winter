//
// Created by Alienson on 1.2.2024..
//

#ifndef WINTER_HTTPRESPONSE_H
#define WINTER_HTTPRESPONSE_H


#include <memory>
#include "HttpRequest.h"
#include "httpConstants.h"
#include "Reflect.h"
#include "JsonSerializer.h"

using namespace std;

class HttpResponse {

public:
    HttpResponse();
    explicit HttpResponse(HttpCode* code);
    explicit HttpResponse(Reflect* data, HttpCode* code = HttpCode::OK);

    void send() const;
    [[nodiscard]] string toResponseString() const;
    [[nodiscard]] string& getBody();
    void setConnection(Connection* _connection);

private:
    HttpVersion* httpVersion = nullptr;
    HttpCode* httpCode = nullptr;
    unordered_map<string,string> responseHeaders;
    string responseBody;

    Connection* connection = nullptr;
    JsonSerializer serializer{};
    static unordered_map<string,string> baseResponseHeaders;

    [[nodiscard]] string writeRequestLine() const;
    [[nodiscard]] string writeRequestHeaders() const;
    [[nodiscard]] string writeRequestBody() const;
};


#endif //WINTER_HTTPRESPONSE_H
