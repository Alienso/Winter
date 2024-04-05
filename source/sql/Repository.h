//
// Created by alienson on 22.3.24..
//

#ifndef WINTER_REPOSITORY_H
#define WINTER_REPOSITORY_H


#include <../util/ConnectionPool.h>
#include "Statement.h"
#include "Entity.h"

class Repository {

public:
    static shared_ptr<Statement> createStatement(const char* query);
    static shared_ptr<Statement> createStatement(const string& query);
    static shared_ptr<Statement> createStatement();

    static int update(const Entity* entity);
    static int insert(const Entity* entity);
    static int update(const vector<Entity*>& entity);
    static int insert(const vector<Entity*>& entity);
    static int deleteEntity(Entity* e);

private:
    static ConnectionPool* dbConnectionPool;

    [[nodiscard]] static string getInsertColumnNames(const Entity* e);
    [[nodiscard]] static string toInsertString(const Entity* e);
    [[nodiscard]] static string toUpdateString(const Entity* e);

    [[nodiscard]] static string getPrimaryKeyValue(const Entity* e);
};


#endif //WINTER_REPOSITORY_H
