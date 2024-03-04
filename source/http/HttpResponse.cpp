//
// Created by Alienson on 1.2.2024..
//

#include "HttpResponse.h"
#include "Connection.h"
#include "../log/Logger.h"

HttpResponse::HttpResponse() {

}

HttpResponse::HttpResponse(Reflect *data, HttpCode *code) : httpCode(code) {
    responseBody = *(serializer.serialize(data)); //TODO this is a copy
}

string &HttpResponse::getBody() {
    return responseBody;
}

shared_ptr<string> HttpResponse::toResponseString(){

    string requestLine = writeRequestLine();
    string headers = writeRequestHeaders();
    string body = writeRequestBody();

    string response = requestLine + headers + body + "\n";
    return make_shared<string>(response);
}

void HttpResponse::send() {
    shared_ptr<string> response = toResponseString();
    connection->respondToHttpRequest(*response);
}

HttpResponse HttpResponse::generateResponse(const shared_ptr<HttpRequest>& httpRequest) {
    HttpResponse response;
    response.httpVersion = HttpVersion::V1_1;
    response.httpCode = HttpCode::OK;

    response.responseHeaders["Date"] = "Mon, 27 Jul 2009 12:28:53 GMT";
    response.responseHeaders["Connection"] = "Closed";
    response.responseHeaders["Server"] = "WT/0.0.1";

    response.connection = httpRequest->getConnection();
    response.responseBody = "This is the body!";
    return response;
}

string HttpResponse::writeRequestLine() {
    return httpVersion->name + ' ' + to_string(httpCode->code) + ' ' + httpCode->name + '\n';
}

string HttpResponse::writeRequestHeaders() {
    vector<char> result; //TODO
    for (const auto& x : responseHeaders){

        for (char c: x.first)
            result.push_back(c);

        result.push_back(':');
        result.push_back(' ');

        for (char c: x.second)
            result.push_back(c);

        result.push_back('\n');
    }

    result.push_back('\n');
    result.push_back('\0');
    return {result.data()};
}

string HttpResponse::writeRequestBody() {
    return responseBody;
}
