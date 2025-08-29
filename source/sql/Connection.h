//
// Created by Alienson on 14.3.2024.
//

#ifndef WINTER_CONNECTION_H
#define WINTER_CONNECTION_H


#include "Statement.h"
#include <memory>

class Connection {
public:
    virtual std::shared_ptr<Statement> createStatement(std::shared_ptr<Connection> connection) = 0;
    virtual std::shared_ptr<Statement> createStatement(std::shared_ptr<Connection> connection, const char* s) = 0;

    virtual void commit() = 0;
    virtual void rollback() = 0;
    virtual void close() = 0;
};


#endif //WINTER_CONNECTION_H
