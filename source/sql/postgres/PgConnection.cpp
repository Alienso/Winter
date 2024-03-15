//
// Created by Alienson on 14.3.2024..
//

#include "PgConnection.h"

shared_ptr<Statement> PgConnection::createStatement() {
    return shared_ptr<Statement>();
}

shared_ptr<Statement> PgConnection::prepareStatement(string s) {
    return shared_ptr<Statement>();
}

shared_ptr<Statement> PgConnection::prepareStatement(const char *s) {
    return shared_ptr<Statement>();
}

void PgConnection::commit() {

}

void PgConnection::rollback() {

}

void PgConnection::close() {

}
