//
// Created by Alienson on 14.3.2024..
//

#include "PgConnection.h"
#include "Logger.h"
#include "PgStatement.h"

PgConnection::PgConnection(const string& connectionString) {
    PGconn *conn;
    conn = PQconnectdb(connectionString.data());
    if (PQstatus(conn) != CONNECTION_OK){
        wtLogError("%s", PQerrorMessage(conn));
        PQfinish(conn);
    }
    postgresConn = conn;
}

PgConnection::~PgConnection() {
    PQfinish(postgresConn);
}

shared_ptr<Statement> PgConnection::createStatement(shared_ptr<Connection> connection) {
    return make_shared<PgStatement>(connection); //'this' and connection are the same
}

shared_ptr<Statement> PgConnection::createStatement(shared_ptr<Connection> connection, const char *s) {
    return make_shared<PgStatement>(connection, s); //'this' and connection are the same
}

void PgConnection::commit() {
    PGresult* res = PQexec(postgresConn, "END");
    validateResponse(postgresConn, res, "Error while committing transaction");
}

void PgConnection::rollback() {
    PGresult* res = PQexec(postgresConn, "ROLLBACK");
    validateResponse(postgresConn, res, "Error while committing transaction");
}

void PgConnection::close() {
    wtLogInfo("Closing PgConnection!");
    PQfinish(postgresConn);
}