//
// Created by Alienson on 14.3.2024..
//

#include "PgStatement.h"
#include "PgResultSet.h"

PgStatement::~PgStatement() {
    PGconn* conn = ((PgConnection*)connection.get())->postgresConn;
    PGresult* res = PQexec(conn, "END");
    if (validateResponse(conn, res, "Error while committing transaction"))
        PQclear(res);
}

shared_ptr<ResultSet> PgStatement::executeQuery(const char *s) {
    PGconn* conn = ((PgConnection*)connection.get())->postgresConn;
    PGresult *res;

    res = PQexec(conn, "BEGIN");
    if (!validateResponse(conn, res, "Error while creating transaction"))
        return nullptr;
    PQclear(res);

    res = PQexec(conn, s);
    if (!validateResponse(conn, res, "Error while executing query"))
        return nullptr;
    return shared_ptr<ResultSet>(new PgResultSet(res, conn));
}

int PgStatement::executeUpdate(const char *s) {
    PGconn* conn = ((PgConnection*)connection.get())->postgresConn;
    PGresult *res;

    res = PQexec(conn, "BEGIN");
    if (!validateResponse(conn, res, "Error while creating transaction"))
        return -1;
    PQclear(res);

    res = PQexec(conn, s);
    if (!validateResponse(conn, res, "Error while executing query"))
        return -1;

    char *rowsAffected = PQgetvalue(res, 0, 0);
    return stoi(rowsAffected);
}

void PgStatement::setQueryTimeout(int milliseconds) {
    PGconn* conn = ((PgConnection*)connection.get())->postgresConn;
    PGresult* res = PQexec(conn, ("SET statement_timeout = " + to_string(milliseconds)).data());
    if (!validateResponse(conn, res, "Error while setting timeout"))
        return;
    PQclear(res);
}

void PgStatement::close() {
    wtLogInfo("Closing PgStatement is not implemented!");
}