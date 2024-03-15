//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_CONNECTION_H
#define WINTER_CONNECTION_H


#include "Statement.h"
#include <memory>

using namespace std;

class Connection {
public:
    virtual shared_ptr<Statement> createStatement() = 0;
    virtual shared_ptr<Statement> prepareStatement(string s) = 0;
    virtual shared_ptr<Statement> prepareStatement(const char* s) = 0;

    virtual void commit() = 0;
    virtual void rollback() = 0;
    virtual void close() = 0;
};


#endif //WINTER_CONNECTION_H
