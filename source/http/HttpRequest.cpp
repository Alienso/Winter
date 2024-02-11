//
// Created by Alienson on 25.1.2024..
//

#include "HttpRequest.h"
#include "Connection.h"
#include "../log/Logger.h"
#include "../util/stringUtils.h"

HttpRequest::HttpRequest() {

}

optional<HttpRequest> HttpRequest::parseFromString(string& data) {
    HttpRequest httpRequest;

    wtLogTrace("Data received: %s", data.data());

    int startIndex, endIndex;
    endIndex = data.find('\n');
    string_view line(data.data(), endIndex + 1); //sizeof \n

    startIndex = endIndex + 1;
    endIndex = data.find("\n\n", endIndex + 1);
    if (endIndex == string::npos)
        endIndex = data.length();
    //string_view headers(&(data[startIndex]), endIndex - startIndex + 2); //sizeof \n\n
    string headers = data.substr(startIndex, endIndex - startIndex + 2);

    startIndex = endIndex;
    endIndex = data.length();
    if (endIndex == string::npos)
        endIndex = data.length();
    string_view body(&(data[startIndex]), endIndex - startIndex);

    parseRequestLine(httpRequest, line);
    parseRequestHeaders(httpRequest, headers);
    parseRequestBody(httpRequest, body);

    wtLogTrace("Parsed http request!");
    return {httpRequest};
}

void HttpRequest::parseRequestLine(HttpRequest &request, string_view line) {
    wtLogTrace("Request Line: ", line.data(), line.length());

    unsigned int startIndex, endIndex;

    endIndex = line.find(' ');
    string_view method(line.data(), endIndex);
    request.method = HttpMethod::getFromString(method.data(), method.size());

    startIndex = endIndex + 1;
    endIndex = line.find(' ', endIndex + 1); //sizeof(' ')
    string_view endpoint(&(line[startIndex]), endIndex - startIndex);
    URI newUri(endpoint);
    request.uri = newUri;
    wtLogTrace("URI: ", request.uri.getPath().data(), request.uri.getPath().size());

    startIndex = endIndex + 1;
    endIndex = line.find('\n', endIndex + 1); //sizeof('\n')
    wtLogTrace("StartIndex: %d, EndIndex: %d", startIndex, endIndex);
    string_view version(&(line[startIndex]), endIndex - startIndex);
    version = rtrim(version);
    request.httpVersion = HttpVersion::getFromString(version.data(), version.size());
    wtLogTrace("HttpVersion is %d", request.httpVersion);
}

void HttpRequest::parseRequestHeaders(HttpRequest &request, string &headers) {
    //wtLogTrace("Headers: ", headers.data(), headers.length());
    string headerValue, headerName;
    int startIndex = 0,endIndex = 0;
    int i;
    while(true){
        //find indexes of headerName
        for(i = startIndex; i<headers.size(); i++){
            if (headers[i] == ':') {
                endIndex = i;
                break;
            }
        }
        //Cannot find another line of x:y
        if (i == headers.size()){
            break;
        }
        headerName = headers.substr(startIndex,endIndex - startIndex);
        //find index of headerValue
        startIndex = endIndex + 1;
        for(i = startIndex; i<headers.size(); i++){
            if (headers[i] == '\n') {
                endIndex = i;
                break;
            }
        }
        if (headers[i - 1] == '\r')
            headerValue = headers.substr(startIndex, endIndex - startIndex - 1);
        else
            headerValue = headers.substr(startIndex, endIndex - startIndex);

        startIndex = endIndex + 1;
        request.requestHeaders[headerName] = headerValue;
    }
}

void HttpRequest::parseRequestBody(HttpRequest &request, string_view body) {
    request.requestBody = body.substr(0, body.size());
}

void HttpRequest::setConnection(Connection* _connection) {
    this->connection = _connection;
}

Connection *HttpRequest::getConnection() {
    return connection;
}
