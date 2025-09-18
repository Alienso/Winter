//
// Created by Alienson on 25.1.2024.
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
#include "http/httpConstants.h"

class HttpConnection;

class HttpRequest {

public:
    HttpRequest();
    HttpRequest(HttpMethod* _method, URI _uri, HttpVersion* _httpVersion, std::unordered_map<std::string, std::string>& _requestHeaders, std::string& _requestBody, HttpConnection* _connection ) :
    method(_method), uri(std::move(_uri)), httpVersion(_httpVersion), requestHeaders(_requestHeaders), requestBody(_requestBody), connection(_connection){}

    [[nodiscard]] static std::shared_ptr<HttpRequest> parseFromString(const std::string &data);

    void setConnection(HttpConnection* _connection);
    [[nodiscard]] HttpConnection* getConnection() const;
    [[nodiscard]] const URI& getUri() const;
    [[nodiscard]] HttpMethod* getMethod() const;
    [[nodiscard]] HttpVersion* getHttpVersion() const;
    [[nodiscard]] const std::unordered_map<std::string, std::string>& getQueryParameters();
    [[nodiscard]] const std::unordered_map<std::string, std::string>& getRequestHeaders();
    [[nodiscard]] const std::string& getRequestBody() const;

private:
    HttpMethod* method;
    URI uri;
    HttpVersion* httpVersion;
    std::unordered_map<std::string, std::string> queryParameters;
    std::unordered_map<std::string, std::string> requestHeaders;
    std::string requestBody;
    HttpConnection* connection;

    static void parseRequestLine(HttpRequest& request, const std::string &line);
    static void parseRequestHeaders(HttpRequest& request, const std::string &headers);
    static void parseRequestBody(HttpRequest& request, std::string_view body);
    static void parseQueryParams(HttpRequest &request, const std::string &basicString, size_t start, size_t index);
};


#endif //WINTER_HTTPREQUEST_H
