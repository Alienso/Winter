//
// Created by alienson on 22.3.24.
//

#include "../core/Configuration.h"
#include "postgres/PgConnection.h"
#include "Repository.h"

ConnectionPool* Repository::dbConnectionPool = new ConnectionPool(2, Configuration::dbConnectionPoolSize,
        []{ return (Connection*) new PgConnection(Configuration::dbConnectionString); }); //TODO

std::shared_ptr<Statement> Repository::createStatement(const char *query) {
    std::shared_ptr<Connection> connection = dbConnectionPool->getConnection();
    return connection->createStatement(connection, query);
}

std::shared_ptr<Statement> Repository::createStatement(const std::string &query) {
    std::shared_ptr<Connection> connection = dbConnectionPool->getConnection();
    return connection->createStatement(connection, query.data());
}

std::shared_ptr<Statement> Repository::createStatement() {
    std::shared_ptr<Connection> connection = dbConnectionPool->getConnection();
    return connection->createStatement(connection);
}

// "id,name,created_on"
std::string Repository::getInsertColumnNames(const Entity* e){
    std::string res;
    std::unordered_map<std::string, std::string>& mappings = e->getColumnMappings();
    for (const auto& it : mappings){
        res += it.second;
        res += ',';
    }
    res.resize(res.size() - 1);
    return res;
}

// "3,'New User',current_timestamp"
std::string Repository::toInsertString(const Entity* e){
    std::string res;
    std::unordered_map<std::string, std::string>& mappings = e->getColumnMappings();
    for (const auto& it : mappings){
        Field* f = e->getField(it.first.data());
        if (f == &Field::INVALID) {
            wtLogError("Invalid field encountered! %s", it.first.data());
            return "";
        }
        std::string val = f->getAsString((Reflect*)e, '\'');
        res+= val;
        res+= ',';
    }
    res.resize(res.size() - 1);
    return res;
}

// "id=4,name='New Name',create_on='NOW'"
std::string Repository::toUpdateString(const Entity* e){
    std::string res;
    std::unordered_map<std::string, std::string>& mappings = e->getColumnMappings();
    for(auto& it : mappings){
        res += it.second;
        res += '=';

        Field* f = e->getField(it.first.data());
        if (f == &Field::INVALID) {
            wtLogError("Invalid field encountered! %s", it.first.data());
            return "";
        }
        std::string val = f->getAsString((Reflect*)e, '\'');

        res += val;
        res += ',';
    }

    res.resize(res.size() - 1);
    res += "where id = " + getPrimaryKeyValue(e);
    return res;
}

int Repository::insert(const Entity *entity) {
    std::string query = "insert into " + entity->getTableName() + "(";
    query+= getInsertColumnNames(entity);
    query+= ") values (";
    query+= toInsertString(entity);
    query+=")";
    return createStatement()->executeUpdate(query.data());
}

int Repository::update(const Entity *entity) {
    std::string query = "update " + entity->getTableName() + " set ";
    query+=toUpdateString(entity);
    return createStatement()->executeUpdate(query.data());
}

int Repository::insert(const std::vector<Entity*>& entities) {

    if (entities.empty())
        return 0;

    std::string query = "insert into " + entities[0]->getTableName() + "(";
    query += getInsertColumnNames(entities[0]);
    query += ") values (";
    for (Entity* e: entities) {
        query += toInsertString(e);
        query += ',';
    }
    query[query.size() - 1] = ')';
    return createStatement()->executeUpdate(query.data());
}

int Repository::update(const std::vector<Entity*>& entities) {

    if (entities.empty())
        return 0;

    int counter = 0;
    for (Entity* e : entities) {
        std::string query = "update " + entities[0]->getTableName() + " set ";
        query += toUpdateString(e);
        if(createStatement()->executeUpdate(query.data()) != 1)
            wtLogError("Error while executing query: %s", query.data());
        else counter++;
    }

    return counter;
}

int Repository::deleteEntity(Entity* e){
    std::string id = getPrimaryKeyValue(e);
    std::string query = "delete from " + e->getTableName() + " where id = " + id;
    return createStatement()->executeUpdate(query.data());
}

std::string Repository::getPrimaryKeyValue(const Entity* e){
    std::unordered_map<std::string, std::string>& mappings = e->getColumnMappings();
    auto it = mappings.find(e->getPrimaryKeyName());
    if (it == mappings.end())
        return ";";

    Field* f = e->getField(it->first.data());
    if (f == &Field::INVALID) {
        wtLogError("Invalid field encountered! %s", e->getPrimaryKeyName().data());
        return ";";
    }

    return f->getAsString((Reflect*)e, '\'');
}
