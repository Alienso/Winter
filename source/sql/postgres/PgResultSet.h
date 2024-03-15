//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_PGRESULTSET_H
#define WINTER_PGRESULTSET_H


#include "../ResultSet.h"

class PgResultSet : public ResultSet {
    bool next() override;

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

    Reflect *getResult(int columnIndex) override;

    vector<Reflect*> getResultList(int columnIndex) override;
};


#endif //WINTER_PGRESULTSET_H
