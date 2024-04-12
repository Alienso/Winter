//
// Created by alienson on 6.4.24..
//

#ifndef WINTER_HTTPBASE_H
#define WINTER_HTTPBASE_H


#include "httpConstants.h"
#include "Connection.h"

#include <unordered_map>

using namespace std;

class HttpBase {
public:
    HttpBase()= default;
    virtual ~HttpBase() = default;

    [[nodiscard]] string toResponseString() const;

protected:
    HttpVersion* httpVersion = HttpVersion::V1_1;
    unordered_map<string,string> httpHeaders;
    string httpBody;
    wt::web::Connection* connection = nullptr;

private:
    [[nodiscard]] virtual string writeRequestLine() const = 0;
    [[nodiscard]] string writeRequestHeaders() const;
    [[nodiscard]] string writeRequestBody() const;
};


#endif //WINTER_HTTPBASE_H
