//
// Created by Alienson on 2.2.2024.
//

#include "httpConstants.h"
#include "Logger.h"
#include "stringUtils.h"

#include <cstring>

HttpCode *HttpCode::OK = new HttpCode(200,"OK");
HttpCode *HttpCode::BAD_REQUEST = new HttpCode(400,"Bad Request");
HttpCode *HttpCode::NOT_FOUND = new HttpCode(404,"Not found");
HttpCode *HttpCode::METHOD_NOT_ALLOWED = new HttpCode(405,"Method not allowed");
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
HttpMethod* HttpMethod::PATCH = new HttpMethod("PATCH");

HttpVersion* HttpVersion::fromString(const char* s){
    wtLogTrace("Http version string: %s", s);
    if (StringUtils::startsWith(s,"HTTP/1.0"))
        return HttpVersion::V1_0;
    else if (StringUtils::startsWith(s,"HTTP/1.1"))
        return HttpVersion::V1_1;
    else if (StringUtils::startsWith(s,"HTTP/2.0"))
        return HttpVersion::V2_0;

    wtLogError("Invalid HttpVersion: %s", s);
    return nullptr;
}

HttpMethod* HttpMethod::fromString(const char* s) {
    if (StringUtils::startsWith(s, "GET"))
        return HttpMethod::GET;
    else if (StringUtils::startsWith(s, "POST"))
        return HttpMethod::POST;
    else if (StringUtils::startsWith(s, "HEAD"))
        return HttpMethod::HEAD;
    else if (StringUtils::startsWith(s, "PUT"))
        return HttpMethod::PUT;
    else if (StringUtils::startsWith(s, "DELETE"))
        return HttpMethod::DELETE_;
    else if (StringUtils::startsWith(s, "CONNECT"))
        return HttpMethod::CONNECT;
    else if (StringUtils::startsWith(s, "OPTIONS"))
        return HttpMethod::OPTIONS;
    else if (StringUtils::startsWith(s, "TRACE"))
        return HttpMethod::TRACE;
    else if (StringUtils::startsWith(s, "PATCH"))
        return HttpMethod::PATCH;

    wtLogError("Invalid HttpMethod: %s", s);
    return nullptr;
}
