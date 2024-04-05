//
// Created by alienson on 22.3.24..
//

#include "../core/Configuration.h"
#include "postgres/PgConnection.h"
#include "Repository.h"

ConnectionPool* Repository::dbConnectionPool = new ConnectionPool(2, Configuration::dbConnectionPoolSize,
        []{ return (Connection*) new PgConnection(Configuration::dbConnectionString); }); //TODO

shared_ptr<Statement> Repository::createStatement(const char *query) {
    shared_ptr<Connection> connection = dbConnectionPool->getConnection();
    return connection->createStatement(connection, query);
}

shared_ptr<Statement> Repository::createStatement(const string &query) {
    shared_ptr<Connection> connection = dbConnectionPool->getConnection();
    return connection->createStatement(connection, query.data());
}

shared_ptr<Statement> Repository::createStatement() {
    shared_ptr<Connection> connection = dbConnectionPool->getConnection();
    return connection->createStatement(connection);
}

// "id,name,created_on"
string Repository::getInsertColumnNames(const Entity* e){
    string res;
    unordered_map<string, string>& mappings = e->getColumnMappings();
    for (const auto& it : mappings){
        res += it.second;
        res += ',';
    }
    res.resize(res.size() - 1);
    return res;
}

// "3,'New User',current_timestamp"
string Repository::toInsertString(const Entity* e){
    string res;
    unordered_map<string, string>& mappings = e->getColumnMappings();
    for (const auto& it : mappings){
        Field* f = e->getField(it.first.data());
        if (f == &Field::INVALID) {
            wtLogError("Invalid field encountered! %s", it.first.data());
            return "";
        }
        string val = f->getAsString((Reflect*)e, '\'');
        res+= val;
        res+= ',';
    }
    res.resize(res.size() - 1);
    return res;
}

// "id=4,name='New Name',create_on='NOW'"
string Repository::toUpdateString(const Entity* e){
    string res;
    unordered_map<string, string>& mappings = e->getColumnMappings();
    for(auto& it : mappings){
        res += it.second;
        res += '=';

        Field* f = e->getField(it.first.data());
        if (f == &Field::INVALID) {
            wtLogError("Invalid field encountered! %s", it.first.data());
            return "";
        }
        string val = f->getAsString((Reflect*)e, '\'');

        res += val;
        res += ',';
    }

    res.resize(res.size() - 1);
    res += "where id = " + getPrimaryKeyValue(e);
    return res;
}

int Repository::insert(const Entity *entity) {
    string query = "insert into " + entity->getTableName() + "(";
    query+= getInsertColumnNames(entity);
    query+= ") values (";
    query+= toInsertString(entity);
    query+=")";
    return createStatement()->executeUpdate(query.data());
}

int Repository::update(const Entity *entity) {
    string query = "update " + entity->getTableName() + " set ";
    query+=toUpdateString(entity);
    return createStatement()->executeUpdate(query.data());
}

int Repository::insert(const vector<Entity*>& entities) {

    if (entities.empty())
        return 0;

    string query = "insert into " + entities[0]->getTableName() + "(";
    query += getInsertColumnNames(entities[0]);
    query += ") values (";
    for (Entity* e: entities) {
        query += toInsertString(e);
        query += ',';
    }
    query[query.size() - 1] = ')';
    return createStatement()->executeUpdate(query.data());
}

int Repository::update(const vector<Entity*>& entities) {

    if (entities.empty())
        return 0;

    int counter = 0;
    for (Entity* e : entities) {
        string query = "update " + entities[0]->getTableName() + " set ";
        query += toUpdateString(e);
        if(createStatement()->executeUpdate(query.data()) != 1)
            wtLogError("Error while executing query: %s", query.data());
        else counter++;
    }

    return counter;
}

int Repository::deleteEntity(Entity* e){
    string id = getPrimaryKeyValue(e);
    string query = "delete from " + e->getTableName() + " where id = " + id;
    return createStatement()->executeUpdate(query.data());
}

string Repository::getPrimaryKeyValue(const Entity* e){
    unordered_map<string, string>& mappings = e->getColumnMappings();
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
