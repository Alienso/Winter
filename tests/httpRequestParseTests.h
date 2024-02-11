//
// Created by Alienson on 31.1.2024..
//

#ifndef WINTER_HTTPREQUESTPARSETESTS_H
#define WINTER_HTTPREQUESTPARSETESTS_H

#include "../source/http/HttpRequest.h"

void parseRequestLine(){
    string requestLine = "GET /some/endpoint HTTP/1.1\n\n\n";
    optional<HttpRequest> request = HttpRequest::parseFromString(requestLine);
}

void runHttpRequestParseTests(){
    parseRequestLine();
}

#endif //WINTER_HTTPREQUESTPARSETESTS_H
