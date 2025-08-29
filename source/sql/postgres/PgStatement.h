//
// Created by Alienson on 14.3.2024.
//

#ifndef WINTER_PGSTATEMENT_H
#define WINTER_PGSTATEMENT_H


#include "../Statement.h"

#include <utility>
#include "PgConnection.h"

class PgStatement : public Statement {
public:

    PgStatement()= default;
    explicit PgStatement(std::shared_ptr<Connection> connection_){
        connection = std::move(connection_);
    }
    PgStatement(std::shared_ptr<Connection> connection_, const char* s){
        connection = std::move(connection_);
        query = s;
        generateParameterMap();
    }
    ~PgStatement() override;

    std::shared_ptr<ResultSet> executeQuery(const char *s) override;

    int executeUpdate(const char *s) override;

    void setQueryTimeout(int milliseconds) override;

    void close() override;

private:
    std::shared_ptr<Connection> connection = nullptr;
};


#endif //WINTER_PGSTATEMENT_H
