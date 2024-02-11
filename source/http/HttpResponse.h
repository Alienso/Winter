//
// Created by Alienson on 1.2.2024..
//

#ifndef WINTER_HTTPRESPONSE_H
#define WINTER_HTTPRESPONSE_H


#include <memory>
#include "HttpRequest.h"
#include "httpConstants.h"

using namespace std;

class HttpResponse {

public:
    HttpResponse();

    void send();
    shared_ptr<string> toResponseString();

    static HttpResponse generateResponse(const shared_ptr<HttpRequest>& sharedPtr);

private:
    HttpVersion* httpVersion;
    HttpCode* httpCode;
    unordered_map<string,string> responseHeaders;
    string responseBody;
    Connection* connection;

    string writeRequestLine();
    string writeRequestHeaders();
    string writeRequestBody();
};


#endif //WINTER_HTTPRESPONSE_H
