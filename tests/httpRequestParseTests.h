//
// Created by Alienson on 31.1.2024..
//

#ifndef WINTER_HTTPREQUESTPARSETESTS_H
#define WINTER_HTTPREQUESTPARSETESTS_H

#include "../source/http/HttpRequest.h"


TEST_CASE("Parsing request line HttpRequest", "[HttpRequest::parseFromString]"){
    SECTION("Parsing GET http request") {
        string requestLine = "GET /some/endpoint HTTP/1.1\n\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/some/endpoint");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().empty());
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().empty());
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
        REQUIRE(request->getQueryParameters().empty());
    }

    SECTION("Parsing DELETE http 2.0 request") {
        string requestLine = "DELETE /some/endpoint HTTP/2.0\n\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/some/endpoint");
        REQUIRE(request->getMethod() == HttpMethod::DELETE_);
        REQUIRE(request->getRequestHeaders().empty());
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V2_0);
        REQUIRE(request->getQueryParameters().empty());
    }

    SECTION("Parsing single query parameter") {
        string requestLine = "GET /some/endpoint?value=true HTTP/1.1\n\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/some/endpoint");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().empty());
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().size() == 1);
        REQUIRE(request->getQueryParameters().at("value") == "true");
    }

    SECTION("Parsing multiple query parameter") {
        string requestLine = "GET /some/endpoint?value=true&limit=100 HTTP/1.1\n\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/some/endpoint");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().empty());
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().size() == 2);
        REQUIRE(request->getQueryParameters().at("value") == "true");
        REQUIRE(request->getQueryParameters().at("limit") == "100");
    }
}

TEST_CASE("Parsing httpRequest headers", "[HttpRequest::parseFromString]"){
    SECTION("Parsing single http request header \\r\\n") {
        string requestLine = "GET /home HTTP/1.1\r\nContent-Type: application/json\r\n\r\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);

        std::cout << "REQUEST BODY: " << request->getRequestBody() << '\n';

        REQUIRE(request->getUri().getPath() == "/home");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().size() == 1);
        REQUIRE(request->getRequestHeaders().at("Content-Type") == "application/json");
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().empty());
    }

    SECTION("Parsing single http request header \\n") {
        string requestLine = "GET /home HTTP/1.1\nContent-Type: application/json\r\n\r\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/home");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().size() == 1);
        REQUIRE(request->getRequestHeaders().at("Content-Type") == "application/json");
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().empty());
    }

    SECTION("Parsing multiple http request header \\r\\n") {
        string requestLine = "GET /home HTTP/1.1\r\nContent-Type: application/json\r\nUser-Agent: PostmanRuntime/7.37.0\r\n\r\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/home");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().size() == 2);
        REQUIRE(request->getRequestHeaders().at("Content-Type") == "application/json");
        REQUIRE(request->getRequestHeaders().at("User-Agent") == "PostmanRuntime/7.37.0");
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().empty());
    }

    SECTION("Parsing multiple http request header \\n") {
        string requestLine = "GET /home HTTP/1.1\nContent-Type: application/json\nUser-Agent: PostmanRuntime/7.37.0\r\n\r\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/home");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().size() == 2);
        REQUIRE(request->getRequestHeaders().at("Content-Type") == "application/json");
        REQUIRE(request->getRequestHeaders().at("User-Agent") == "PostmanRuntime/7.37.0");
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().empty());
    }

    SECTION("Parsing multiple http request header without spaces") {
        string requestLine = "GET /home HTTP/1.1\nContent-Type:application/json\nUser-Agent:PostmanRuntime/7.37.0\r\n\r\n";
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestLine);
        REQUIRE(request->getUri().getPath() == "/home");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().size() == 2);
        REQUIRE(request->getRequestHeaders().at("Content-Type") == "application/json");
        REQUIRE(request->getRequestHeaders().at("User-Agent") == "PostmanRuntime/7.37.0");
        REQUIRE(request->getRequestBody().empty());
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().empty());
    }
}

static string requestStr = "GET /home HTTP/1.1\r\nContent-Type: application/json\r\nUser-Agent: PostmanRuntime/7.37.0\r\nAccept: */*\r\nPostman-Token: 1009ba86-d675-4078-bbe0-804420a2bb69\r\nHost: localhost:8080\r\nAccept-Encoding: gzip, deflate, br\r\nConnection: keep-alive\r\nContent-Length: 142";
static string requestBody = "{\n    \"number\": 10,\n    \"type\": \"someType\",\n    \"innerClass\":{\n        \"x\": 2,\n        \"y\": 3,\n        \"c\": \"a\"\n    },\n    \"values\": [1,2,3]\n}";

TEST_CASE("Parsing httpRequest with body", "[HttpRequest::parseFromString]"){

    SECTION("Parsing http request with body and body separator is \\r\\n\\r\\n") {
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestStr + "\r\n\r\n" + requestBody);
        REQUIRE(request->getUri().getPath() == "/home");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().size() == 8);
        REQUIRE(request->getRequestBody() == requestBody);
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().empty());
    }

    SECTION("Parsing http request with body and body separator is \\n\\n") {
        shared_ptr<HttpRequest> request = HttpRequest::parseFromString(requestStr + "\n\n" + requestBody);
        REQUIRE(request->getUri().getPath() == "/home");
        REQUIRE(request->getMethod() == HttpMethod::GET);
        REQUIRE(request->getRequestHeaders().size() == 8);
        REQUIRE(request->getRequestBody() == requestBody);
        REQUIRE(request->getHttpVersion() == HttpVersion::V1_1);
        REQUIRE(request->getQueryParameters().empty());
    }
}

#endif //WINTER_HTTPREQUESTPARSETESTS_H
