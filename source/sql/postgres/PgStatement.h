//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_PGSTATEMENT_H
#define WINTER_PGSTATEMENT_H


#include "../Statement.h"

#include <utility>
#include "PgConnection.h"

class PgStatement : public Statement {
public:

    PgStatement()= default;
    explicit PgStatement(shared_ptr<Connection> connection_){
        connection = std::move(connection_);
    }
    ~PgStatement() override = default;

    shared_ptr<ResultSet> executeQuery(const char *s) override;

    int executeUpdate(const char *s) override;

    void setQueryTimeout(int seconds) override;

    void close() override;

private:
    shared_ptr<Connection> connection = nullptr;
};


#endif //WINTER_PGSTATEMENT_H
