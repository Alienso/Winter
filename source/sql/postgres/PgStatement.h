//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_PGSTATEMENT_H
#define WINTER_PGSTATEMENT_H


#include "../Statement.h"

class PgStatement : public Statement {
public:
    void executeQuery(const char *s) override;

    void executeQuery(string &s) override;

    int executeUpdate(const char *s) override;

    int executeUpdate(string &s) override;

    void setQueryTimeout(int seconds) override;

    void close() override;
};


#endif //WINTER_PGSTATEMENT_H
