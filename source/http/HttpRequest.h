//
// Created by Alienson on 25.1.2024..
//

#ifndef WINTER_HTTPREQUEST_H
#define WINTER_HTTPREQUEST_H

#include "URI.h"
#include "httpConstants.h"
#include "HttpBase.h"

#include <string>
#include <unordered_map>
#include <optional>
#include <utility>
#include <vector>
#include <memory>

class Connection;

using namespace std;

class HttpRequest : public HttpBase{

public:
    HttpRequest();
    HttpRequest(HttpMethod* _method, URI _uri, HttpVersion* _httpVersion, unordered_map<string,string>& _requestHeaders, string& _requestBody, wt::web::Connection* _connection ) :
            httpMethod(_method), uri(std::move(_uri)){
        httpVersion = _httpVersion;
        httpHeaders = _requestHeaders;
        httpBody = _requestBody;
        connection = _connection;
    }

    void send() const;

    [[nodiscard]] static shared_ptr<HttpRequest> parseFromString(const string &data);

    [[nodiscard]] const URI& getUri() const;
    [[nodiscard]] HttpMethod* getMethod() const;
    [[nodiscard]] const unordered_map<string,string>& getQueryParameters();

    [[nodiscard]] const string& getBody() const;
    [[nodiscard]] wt::web::Connection* getConnection() const;
    [[nodiscard]] HttpVersion* getHttpVersion() const;
    [[nodiscard]] unordered_map<string,string>& getHttpHeaders();
    [[nodiscard]] const string& getRequestBody() const;

    void setConnection(wt::web::Connection* _connection);
    void setHttpHeaders(const unordered_map<string,string>& headers);
    void setBody(const string& body);
    void setMethod(HttpMethod* method);
    void setUri(const char* uri);

private:
    HttpMethod* httpMethod;
    URI uri;
    unordered_map<string,string> queryParameters;

    static void parseRequestLine(HttpRequest& request, const string &line);
    static void parseRequestHeaders(HttpRequest& request, const string &headers);
    static void parseRequestBody(HttpRequest& request, string_view body);
    static void parseQueryParams(HttpRequest &request, const string &basicString, size_t start, size_t index);

    string writeRequestLine() const override;
};


#endif //WINTER_HTTPREQUEST_H
