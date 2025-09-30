//
// Created by Alienson on 24.9.2025..
//

#ifndef WINTER_HTTPREQUESTPARSER_H
#define WINTER_HTTPREQUESTPARSER_H

#include "HttpRequest.h"

#include <string>
#include <memory>
#include <mutex>

struct HttpRequestParserProcessStatus{
    bool requestParsed = false;
    bool parsingFailed = false;
};

class HttpRequestParser{
public:
    explicit HttpRequestParser(std::mutex& mutex);

    void parseReceivedData(std::size_t newDataSize);
    [[nodiscard]] const HttpRequestParserProcessStatus& getStatus() const;
    [[nodiscard]] std::shared_ptr<HttpRequest>& getHttpRequest();

    //For test purposes
    void setTempRequestData(const std::string& requestStr);

private:
    void dataReceived(size_t length);
    void handleBodyWithoutLimit(int i);
    void handleBodyWithLimit(int i);

    static void parseRequestLine(HttpRequest& request, const std::string &line);
    static void parseRequestHeaders(HttpRequest& request, const std::string &headers);
    static void parseRequestBody(HttpRequest& request, std::string_view body);
    static void parseQueryParams(HttpRequest &request, const std::string &basicString, size_t start, size_t index);

private:
    std::shared_ptr<HttpRequest> httpRequest{new HttpRequest()};
    HttpRequestParserProcessStatus status;

    char tempRequestBuffer[20 * 1024] = {};
    std::string requestData;

    int headersStartIndex = -1;
    int headersEndIndex = -1;
    int bodySize = -1;
    size_t bodyBytesRead = 0;
    size_t bodyBracketCounter = 0;

    bool readRequestLine = false;
    bool readHeaders = false;
    bool processedHeaders = false;

    std::mutex& processDataMut;

    friend HttpConnection;
};

#endif //WINTER_HTTPREQUESTPARSER_H
