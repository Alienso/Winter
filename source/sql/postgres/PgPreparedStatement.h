//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_PGPREPAREDSTATEMENT_H
#define WINTER_PGPREPAREDSTATEMENT_H


#include "../Statement.h"
#include "PgStatement.h"

class PgPreparedStatement : public PreparedStatement, PgStatement {

public:
    PreparedStatement *setInt(int x, int pos) override;

    PreparedStatement *setLong(long x, int pos) override;

    PreparedStatement *setFloat(float x, int pos) override;

    PreparedStatement *setDouble(double x, int pos) override;

    PreparedStatement *setString(string &s, int pos) override;

    PreparedStatement *setString(const char *s, int pos) override;

    PreparedStatement *setObject(int o, int pos) override;

    PreparedStatement *setNull(int pos) override;

    PreparedStatement *setBool(bool b, int pos) override;

    PreparedStatement *setByte(byte c, int pos) override;

    PreparedStatement *setShort(short s, int pos) override;

    PreparedStatement *setDate(string &s, int pos) override;

    PreparedStatement *setDate(const char *s, int pos) override;

    PreparedStatement *setTime(string &s, int pos) override;

    PreparedStatement *setTime(const char *s, int pos) override;

    PreparedStatement *setDateTime(string &s, int pos) override;

    PreparedStatement *setDateTime(const char *s, int pos) override;

    PreparedStatement *setBlob(string &s, int pos) override;

    PreparedStatement *setBlob(const char *s, int pos) override;

};


#endif //WINTER_PGPREPAREDSTATEMENT_H
