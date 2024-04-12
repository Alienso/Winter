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

class HttpResponse : public HttpBase{

public:
    HttpResponse();
    explicit HttpResponse(HttpStatus* code);
    explicit HttpResponse(Reflect* data, HttpStatus* code = HttpStatus::OK);

    void send() const;
    void setConnection(wt::web::Connection* _connection);

private:
    HttpStatus* httpCode = nullptr;
    static unordered_map<string,string> baseResponseHeaders;

    string writeRequestLine() const override;
};


#endif //WINTER_HTTPRESPONSE_H
