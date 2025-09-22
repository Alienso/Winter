//
// Created by Alienson on 14.3.2024.
//

#include "sql/postgres/PgConnection.h"
#include "log/Loggy.h"
#include "sql/postgres/PgStatement.h"

PgConnection::PgConnection(const std::string& connectionString) {
    PGconn *conn;
    conn = PQconnectdb(connectionString.data());
    if (PQstatus(conn) != CONNECTION_OK){
        wtLogError("{}", PQerrorMessage(conn));
        PQfinish(conn);
    }
    postgresConn = conn;
}

PgConnection::~PgConnection() {
    PQfinish(postgresConn);
}

std::shared_ptr<Statement> PgConnection::createStatement(std::shared_ptr<DbConnection> connection) {
    return std::make_shared<PgStatement>(connection); //'this' and connection are the same
}

std::shared_ptr<Statement> PgConnection::createStatement(std::shared_ptr<DbConnection> connection, const char *s) {
    return std::make_shared<PgStatement>(connection, s); //'this' and connection are the same
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