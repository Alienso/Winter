//
// Created by alienson on 22.3.24..
//

#ifndef WINTER_REPOSITORY_H
#define WINTER_REPOSITORY_H


#include <../util/ConnectionPool.h>
#include "Statement.h"

class Repository {

public:
    static shared_ptr<Statement> createStatement(const char* query);
    static shared_ptr<Statement> createStatement(const string& query);
    static shared_ptr<Statement> createStatement();

private:
    static ConnectionPool* dbConnectionPool;
};


#endif //WINTER_REPOSITORY_H
