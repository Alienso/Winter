//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_PGRESULTSET_H
#define WINTER_PGRESULTSET_H


#include "../ResultSet.h"
#include <libpq-fe.h>

class PgResultSet : public ResultSet {

public:
    PgResultSet(PGresult* pgResult_, PGconn* conn);
    virtual ~PgResultSet();

    bool next() override;
    [[nodiscard]] Reflect *getResult(Reflect* (*allocator)()) const override;
    [[nodiscard]] vector<Reflect*>* getResultList(Reflect* (*allocator)()) const override;

    [[nodiscard]] int getInt(size_t columnIndex) const override;
    [[nodiscard]] long getLong(size_t columnIndex) const override;
    [[nodiscard]] float getFloat(size_t columnIndex) const override;
    [[nodiscard]] double getDouble(size_t columnIndex) const override;
    [[nodiscard]] string getString(size_t columnIndex) const override;
    [[nodiscard]] bool getBool(size_t columnIndex) const override;
    [[nodiscard]] std::byte getByte(size_t columnIndex) const override;
    [[nodiscard]] short getShort(size_t columnIndex) const override;
    [[nodiscard]] string getDate(size_t columnIndex) const override;
    [[nodiscard]] string getTime(size_t columnIndex) const override;
    [[nodiscard]] string getDateTime(size_t columnIndex) const override;
    [[nodiscard]] string getBlob(size_t columnIndex) const override;

private:
    PGresult *pgResult;
    PGconn* pgConn;
    size_t resultSize;
    size_t fieldCount;
    size_t cursorIndex;
};


#endif //WINTER_PGRESULTSET_H
