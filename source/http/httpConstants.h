//
// Created by Alienson on 1.2.2024..
//

#ifndef WINTER_HTTPCONSTANTS_H
#define WINTER_HTTPCONSTANTS_H

#include "string"

using namespace std;

class HttpCode{
public:
    HttpCode(int _code, const char* _name) : code(_code), name(_name) {}
    const int code;
    const string name;

    static HttpCode* OK;
    static HttpCode* BAD_REQUEST;
    static HttpCode* NOT_FOUND;
    static HttpCode* INTERNAL_SERVER_ERROR;
    static HttpCode* METHOD_NOT_ALLOWED;
};

class HttpVersion{
public:
    explicit HttpVersion(const char* _name) : name(_name){}
    const string name;

    [[nodiscard]] static HttpVersion* fromString(const char* s);

    static HttpVersion* V1_0;
    static HttpVersion* V1_1;
    static HttpVersion* V2_0;
};

class HttpMethod{
public:
    explicit HttpMethod(const char* _method) : method(_method) {}
    const string method;

    [[nodiscard]] static HttpMethod* fromString(const char* s);

    static HttpMethod* GET;
    static HttpMethod* POST;
    static HttpMethod* HEAD;
    static HttpMethod* PUT;
    static HttpMethod* DELETE_; //???
    static HttpMethod* CONNECT;
    static HttpMethod* OPTIONS;
    static HttpMethod* TRACE;
    static HttpMethod* PATCH;

};

#endif //WINTER_HTTPCONSTANTS_H
