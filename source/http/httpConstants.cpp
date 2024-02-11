//
// Created by Alienson on 2.2.2024..
//

#include "httpConstants.h"
#include "../log/Logger.h"
#include <cstring>

HttpCode *HttpCode::OK = new HttpCode(200,"OK");
HttpCode *HttpCode::BAD_REQUEST = new HttpCode(400,"Bad Request");
HttpCode *HttpCode::NOT_FOUND = new HttpCode(404,"Not found");
HttpCode *HttpCode::INTERNAL_SERVER_ERROR = new HttpCode(500,"Internal server error");

HttpVersion* HttpVersion::V1_0 = new HttpVersion("HTTP/1.0");
HttpVersion* HttpVersion::V1_1 = new HttpVersion("HTTP/1.1");
HttpVersion* HttpVersion::V2_0 = new HttpVersion("HTTP/2.0");

HttpMethod* HttpMethod::GET = new HttpMethod("GET");
HttpMethod* HttpMethod::POST = new HttpMethod("POST");
HttpMethod* HttpMethod::HEAD = new HttpMethod("HEAD");
HttpMethod* HttpMethod::PUT = new HttpMethod("PUT");
HttpMethod* HttpMethod::DELETE_ = new HttpMethod("DELETE");
HttpMethod* HttpMethod::CONNECT = new HttpMethod("CONNECT");
HttpMethod* HttpMethod::OPTIONS = new HttpMethod("OPTIONS");
HttpMethod* HttpMethod::TRACE = new HttpMethod("TRACE");

HttpVersion* HttpVersion::getFromString(const char* s, size_t length){
    wtLogTrace("Http version string: ", s, length);
    if (strcmp(s,"HTTP/1.0") == 0)
        return HttpVersion::V1_0;
    else if (strcmp(s,"HTTP/1.1") == 0)
        return HttpVersion::V1_1;
    else if (strcmp(s,"HTTP/2.0") == 0)
        return HttpVersion::V2_0;
    else return nullptr;
}

HttpMethod* HttpMethod::getFromString(const char* s, size_t length) {
    if (strcmp(s, "GET") == 0)
        return HttpMethod::GET;
    else if (strcmp(s, "POST") == 0)
        return HttpMethod::POST;
    else if (strcmp(s, "HEAD") == 0)
        return HttpMethod::HEAD;
    else if (strcmp(s, "PUT") == 0)
        return HttpMethod::PUT;
    else if (strcmp(s, "DELETE") == 0)
        return HttpMethod::DELETE_;
    else if (strcmp(s, "CONNECT") == 0)
        return HttpMethod::CONNECT;
    else if (strcmp(s, "OPTIONS") == 0)
        return HttpMethod::OPTIONS;
    else if (strcmp(s, "TRACE") == 0)
        return HttpMethod::TRACE;
    return nullptr;
}
