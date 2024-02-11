//
// Created by Alienson on 1.2.2024..
//

#ifndef WINTER_HTTPCONSTANTS_H
#define WINTER_HTTPCONSTANTS_H

#include "string"

using namespace std;

class HttpCode{
public:
    HttpCode(int _code, const char* _name){
        this->code = _code;
        this->name = string(_name);
    }
    int code{};
    string name;

    static HttpCode* OK;
    static HttpCode* BAD_REQUEST;
    static HttpCode* NOT_FOUND;
    static HttpCode* INTERNAL_SERVER_ERROR;
};

class HttpVersion{
public:
    explicit HttpVersion(const char* _name){
        this->name = string(_name);
    }
    string name;

    static HttpVersion* getFromString(const char* s, size_t length);

    static HttpVersion* V1_0;
    static HttpVersion* V1_1;
    static HttpVersion* V2_0;
};

class HttpMethod{
public:
    explicit HttpMethod(const char* _method){
        this->method = string(_method);
    }
    string method;

    static HttpMethod* getFromString(const char* s, size_t length);

    static HttpMethod* GET;
    static HttpMethod* POST;
    static HttpMethod* HEAD;
    static HttpMethod* PUT;
    static HttpMethod* DELETE_; //???
    static HttpMethod* CONNECT;
    static HttpMethod* OPTIONS;
    static HttpMethod* TRACE;

};

#endif //WINTER_HTTPCONSTANTS_H
