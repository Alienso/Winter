//
// Created by Alienson on 25.1.2024..
//

#ifndef WINTER_HTTPREQUEST_H
#define WINTER_HTTPREQUEST_H

#include <string>
#include <unordered_map>
#include <optional>
#include <utility>
#include <vector>
#include <memory>
#include "URI.h"
#include "httpConstants.h"

class Connection;

using namespace std;

class HttpRequest {

public:
    HttpRequest();
    HttpRequest(HttpMethod* _method, URI _uri, HttpVersion* _httpVersion, unordered_map<string,string>& _requestHeaders, string& _requestBody, Connection* _connection ) :
    method(_method), uri(std::move(_uri)), httpVersion(_httpVersion), requestHeaders(_requestHeaders), requestBody(_requestBody), connection(_connection){}

    [[nodiscard]] static shared_ptr<HttpRequest> parseFromString(string &data);

    void setConnection(Connection* _connection);
    [[nodiscard]] Connection* getConnection() const;
    [[nodiscard]] URI& getUri();
    [[nodiscard]] HttpMethod* getMethod() const;
    [[nodiscard]] string& getRequestBody();

private:
    HttpMethod* method;
    URI uri;
    HttpVersion* httpVersion;
    unordered_map<string,string> requestHeaders;
    string requestBody;
    Connection* connection;

    static void parseRequestLine(HttpRequest& request, string &line);
    static void parseRequestHeaders(HttpRequest& request, string &headers);
    static void parseRequestBody(HttpRequest& request, string_view body);

};


#endif //WINTER_HTTPREQUEST_H
