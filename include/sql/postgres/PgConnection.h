//
// Created by Alienson on 14.3.2024.
//

#ifndef WINTER_PGCONNECTION_H
#define WINTER_PGCONNECTION_H


#include "sql/DbConnection.h"
#include "libpq-fe.h"

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

class PgConnection : public DbConnection{

public:
    explicit PgConnection(const std::string& connectionString);
    virtual ~PgConnection();

    std::shared_ptr<Statement> createStatement(std::shared_ptr<DbConnection> connection) override;

    std::shared_ptr<Statement> createStatement(std::shared_ptr<DbConnection> connection, const char *s) override;

    void commit() override;

    void rollback() override;

    void close() override;

private:
    friend class PgStatement;
    PGconn *postgresConn;
};


#endif //WINTER_PGCONNECTION_H
