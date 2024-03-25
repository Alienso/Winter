//
// Created by alienson on 22.3.24..
//

#include "../Configuration.h"
#include "postgres/PgConnection.h"
#include "Repository.h"

ConnectionPool* Repository::dbConnectionPool = new ConnectionPool(2, Configuration::dbConnectionPoolSize,
        []{ return (Connection*) new PgConnection(Configuration::dbConnectionString); }); //TODO

//TODO

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
