//
// Created by Alienson on 1.2.2024.
//

#ifndef WINTER_HTTPRESPONSE_H
#define WINTER_HTTPRESPONSE_H


#include <memory>
#include "HttpRequest.h"
#include "httpConstants.h"
#include "Reflect.h"
#include "JsonSerializer.h"

class HttpResponse {

public:
    HttpResponse();
    explicit HttpResponse(HttpCode* code);
    explicit HttpResponse(Reflect* data, HttpCode* code = HttpCode::OK);

    void send() const;
    [[nodiscard]] std::string toResponseString() const;
    [[nodiscard]] const std::string& getBody() const;
    void setConnection(Connection* _connection);

private:
    HttpVersion* httpVersion = nullptr;
    HttpCode* httpCode = nullptr;
    std::unordered_map<std::string,std::string> responseHeaders;
    std::string responseBody;

    Connection* connection = nullptr;
    JsonSerializer serializer{};
    static std::unordered_map<std::string,std::string> baseResponseHeaders;

    [[nodiscard]] std::string writeRequestLine() const;
    [[nodiscard]] std::string writeRequestHeaders() const;
    [[nodiscard]] std::string writeRequestBody() const;
};


#endif //WINTER_HTTPRESPONSE_H
