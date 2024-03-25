//
// Created by Alienson on 14.3.2024..
//

#include "PgStatement.h"
#include "PgResultSet.h"

ResultSet* PgStatement::executeQuery(const char *s) {
    PGconn* conn = ((PgConnection*)connection.get())->postgresConn;
    PGresult *res;

    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
    }
    PQclear(res);

    res = PQexec(conn, s);
    return new PgResultSet(res, conn);
}

int PgStatement::executeUpdate(const char *s) {
    return 0;
}


void PgStatement::setQueryTimeout(int seconds) {

}

void PgStatement::close() {

}
