//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_PGCONNECTION_H
#define WINTER_PGCONNECTION_H


#include "../Connection.h"

class PgConnection : public Connection{

public:
    shared_ptr<Statement> createStatement() override;

    shared_ptr<Statement> prepareStatement(string s) override;

    shared_ptr<Statement> prepareStatement(const char *s) override;

    void commit() override;

    void rollback() override;

    void close() override;

};


#endif //WINTER_PGCONNECTION_H
