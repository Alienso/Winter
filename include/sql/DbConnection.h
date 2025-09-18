//
// Created by Alienson on 14.3.2024.
//

#ifndef WINTER_SQL_CONNECTION_H
#define WINTER_SQL_CONNECTION_H


#include "sql/Statement.h"
#include <memory>

class DbConnection {
public:
    virtual std::shared_ptr<Statement> createStatement(std::shared_ptr<DbConnection> connection) = 0;
    virtual std::shared_ptr<Statement> createStatement(std::shared_ptr<DbConnection> connection, const char* s) = 0;

    virtual void commit() = 0;
    virtual void rollback() = 0;
    virtual void close() = 0;
};


#endif //WINTER_SQL_CONNECTION_H
