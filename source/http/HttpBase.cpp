//
// Created by alienson on 6.4.24..
//

#include "HttpBase.h"

string HttpBase::toResponseString() const{

    string requestLine = writeRequestLine();
    string headers = writeRequestHeaders();
    string body = writeRequestBody();

    return requestLine + headers + body + "\n";
}

string HttpBase::writeRequestHeaders() const {
    string result;
    for (const auto& x : httpHeaders){

        for (char c: x.first)
            result.push_back(c);

        result.push_back(':');
        result.push_back(' ');

        for (char c: x.second)
            result.push_back(c);

        result.push_back('\n');
    }

    result.push_back('\n');
    return result;
}

string HttpBase::writeRequestBody() const {
    return httpBody;
}