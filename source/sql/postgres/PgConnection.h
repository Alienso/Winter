//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_PGCONNECTION_H
#define WINTER_PGCONNECTION_H


#include "../Connection.h"
#include <libpq-fe.h>

using namespace wt::sql;

inline bool validateResponse(const PGconn *conn, PGresult *res, const char *message){
    auto code = PQresultStatus(res);
    if (code != PGRES_COMMAND_OK && code != PGRES_TUPLES_OK){
        wtLogError(message);
        wtLogError("SQLERROR: %s", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }
    return true;
}

class PgConnection : public Connection{

public:
    explicit PgConnection(const string& connectionString);
    virtual ~PgConnection();

    shared_ptr<Statement> createStatement(shared_ptr<Connection> connection) override;

    shared_ptr<Statement> createStatement(shared_ptr<Connection> connection, const char *s) override;

    void commit() override;

    void rollback() override;

    void close() override;

private:
    friend class PgStatement;
    PGconn *postgresConn;
};


#endif //WINTER_PGCONNECTION_H
