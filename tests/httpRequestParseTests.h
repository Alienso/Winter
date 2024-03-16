//
// Created by Alienson on 31.1.2024..
//

#ifndef WINTER_HTTPREQUESTPARSETESTS_H
#define WINTER_HTTPREQUESTPARSETESTS_H

#include "../source/http/HttpRequest.h"

void parseRequestLine(){
    string requestLine = "GET /some/endpoint HTTP/1.1\n\n\n";
    shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
}

TEST_CASE("Parsing HttpRequest", "[HttpRequest::parseFromString]"){
    SECTION("Parsing empty http request") {
        string requestLine = "GET /some/endpoint HTTP/1.1\n\n\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/some/endpoint");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().empty());
        REQUIRE(request->getRequestBody().empty());
    }
}

#endif //WINTER_HTTPREQUESTPARSETESTS_H
