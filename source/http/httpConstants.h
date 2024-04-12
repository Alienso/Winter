//
// Created by Alienson on 1.2.2024..
//

#ifndef WINTER_HTTPCONSTANTS_H
#define WINTER_HTTPCONSTANTS_H

#include "string"

using namespace std;

class HttpStatus{
public:
    HttpStatus(int _code, const char* _name) : code(_code), name(_name) {}
    const int code;
    const string name;

    static HttpStatus* OK;
    static HttpStatus* BAD_REQUEST;
    static HttpStatus* NOT_FOUND;
    static HttpStatus* INTERNAL_SERVER_ERROR;
    static HttpStatus* METHOD_NOT_ALLOWED;
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
    explicit HttpMethod(const char* _method) : name(_method) {}
    const string name;

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


class MediaType{
public:
    explicit MediaType(const char* type) : value(type){}
    const string value;

    static MediaType* APPLICATION_JSON;
    static MediaType* TEXT_PLAIN;
    static MediaType* TEXT_HTML;
    static MediaType* TEXT_JAVASCRIPT;
    static MediaType* IMAGE_PNG;
    static MediaType* IMAGE_JPEG;
    static MediaType* MULTIPART_FORM_DATA;
    static MediaType* DOCUMENT_PDF;
    //TODO

private:

};

#endif //WINTER_HTTPCONSTANTS_H
