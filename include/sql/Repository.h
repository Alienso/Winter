//
// Created by alienson on 22.3.24.
//

#ifndef WINTER_REPOSITORY_H
#define WINTER_REPOSITORY_H


#include "util/DbConnectionPool.h"
#include "sql/Statement.h"
#include "sql/Entity.h"

#include <memory>

class Repository {

public:
    static std::shared_ptr<Statement> createStatement(const char* query);
    static std::shared_ptr<Statement> createStatement(const std::string& query);
    static std::shared_ptr<Statement> createStatement();

    static int update(const Entity* entity);
    static int insert(const Entity* entity);
    static int update(const std::vector<Entity*>& entity);
    static int insert(const std::vector<Entity*>& entity);
    static int deleteEntity(Entity* e);

private:
    static DbConnectionPool* dbConnectionPool;

    [[nodiscard]] static std::string getInsertColumnNames(const Entity* e);
    [[nodiscard]] static std::string toInsertString(const Entity* e);
    [[nodiscard]] static std::string toUpdateString(const Entity* e);

    [[nodiscard]] static std::string getPrimaryKeyValue(const Entity* e);


public:

    template<typename T>
    static std::shared_ptr<std::vector<std::shared_ptr<T>>> castTo(std::shared_ptr<std::vector<std::shared_ptr<Reflect>>> source){
        auto dest = std::make_shared<std::vector<std::shared_ptr<T>>>();
        dest->reserve(source->size());
        for (std::shared_ptr<Reflect>& elem : *source){
            dest->push_back(std::dynamic_pointer_cast<T>(elem));
        }
        return dest;
    }

};


#endif //WINTER_REPOSITORY_H
