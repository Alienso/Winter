//
// Created by Alienson on 25.1.2024..
//

#ifndef WINTER_HTTPREQUEST_H
#define WINTER_HTTPREQUEST_H

#include <string>
#include <unordered_map>
#include <optional>
#include <vector>
#include "URI.h"
#include "httpConstants.h"

class Connection;

using namespace std;

class HttpRequest {

public:
    HttpRequest();
    void setConnection(Connection* _connection);
    Connection* getConnection();
    static optional<HttpRequest> parseFromString(string &data);

private:
    HttpMethod* method;
    URI uri;
    HttpVersion* httpVersion;
    unordered_map<string,string> requestHeaders;
    string requestBody;
    Connection* connection;

    static void parseRequestLine(HttpRequest& request, string_view line);
    static void parseRequestHeaders(HttpRequest& request, string &headers);
    static void parseRequestBody(HttpRequest& request, string_view body);

};


#endif //WINTER_HTTPREQUEST_H
