//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_PGRESULTSET_H
#define WINTER_PGRESULTSET_H


#include "../ResultSet.h"
#include <libpq-fe.h>

class PgResultSet : public ResultSet {

public:

    PgResultSet(PGresult* pgResult_, PGconn* conn) : pgResult(pgResult_), pgConn(conn){}

    bool next() override;
    Reflect *getResult() override;
    vector<Reflect*>* getResultList() override;

protected:
    int getInt(int columnIndex) override;
    long getLong(int columnIndex) override;
    float getFloat(int columnIndex) override;
    double getDouble(int columnIndex) override;
    string getString(int columnIndex) override;
    bool getBool(int columnIndex) override;
    byte getByte(int columnIndex) override;
    short getShort(int columnIndex) override;
    string getDate(int columnIndex) override;
    string getTime(int columnIndex) override;
    string getDateTime(int columnIndex) override;
    string getBlob(int columnIndex) override;

private:
    PGresult *pgResult;
    PGconn* pgConn;
};


#endif //WINTER_PGRESULTSET_H
