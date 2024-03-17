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

static string requestStr = "GET /home HTTP/1.1\r\nContent-Type: application/json\r\nUser-Agent: PostmanRuntime/7.37.0\r\nAccept: */*\r\nPostman-Token: 1009ba86-d675-4078-bbe0-804420a2bb69\r\nHost: localhost:8080\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: 142\r\n\r\n{\n    \"number\": 10,\n    \"type\": \"someType\",\n    \"innerClass\":{\n        \"x\": 2,\n        \"y\": 3,\n        \"c\": \"a\"\n    },\n    \"values\": [1,2,3]\n}";

TEST_CASE("Parsing request line HttpRequest", "[HttpRequest::parseFromString]"){
    SECTION("Parsing GET http request") {
        string requestLine = "GET /some/endpoint HTTP/1.1\n\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/some/endpoint");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().empty());
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
    }

    SECTION("Parsing POST http request") {
        string requestLine = "POST /some/endpoint HTTP/1.1\n\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/some/endpoint");
        REQUIRE(request->getMethod() == HttpMethod::POST);
        REQUIRE(request->getRequestHeaders().empty());
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
    }

    SECTION("Parsing PATCH http 1.0 request") {
        string requestLine = "PATCH /some/endpoint HTTP/1.0\n\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/some/endpoint");
        REQUIRE(request->getMethod() == HttpMethod::PATCH);
        REQUIRE(request->getRequestHeaders().empty());
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_0);
    }

    SECTION("Parsing DELETE http 2.0 request") {
        string requestLine = "DELETE /some/endpoint HTTP/1.0\n\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/some/endpoint");
        REQUIRE(request->getMethod() == HttpMethod::DELETE_);
        REQUIRE(request->getRequestHeaders().empty());
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V2_0);
    }
}

#endif //WINTER_HTTPREQUESTPARSETESTS_H
