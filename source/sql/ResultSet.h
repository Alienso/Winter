//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_RESULTSET_H
#define WINTER_RESULTSET_H

#include <string>
#include "Reflect.h"

using namespace std;

class ResultSet{
public:
    [[nodiscard]] virtual Reflect* getResult(Reflect* (*allocator)()) const = 0;
    [[nodiscard]] virtual vector<Reflect*>* getResultList(Reflect* (*allocator)()) const = 0;
    virtual bool next() = 0;

    [[nodiscard]] virtual int getInt(size_t columnIndex) const = 0;
    [[nodiscard]] virtual long getLong(size_t columnIndex) const = 0;
    [[nodiscard]] virtual float getFloat(size_t columnIndex) const = 0;
    [[nodiscard]] virtual double getDouble(size_t columnIndex) const = 0;
    [[nodiscard]] virtual string getString(size_t columnIndex) const = 0;
    [[nodiscard]] virtual bool getBool(size_t columnIndex) const = 0;
    [[nodiscard]] virtual std::byte getByte(size_t columnIndex) const = 0;
    [[nodiscard]] virtual short getShort(size_t columnIndex) const = 0;
    [[nodiscard]] virtual string getDate(size_t columnIndex) const = 0;
    [[nodiscard]] virtual string getTime(size_t columnIndex) const = 0;
    [[nodiscard]] virtual string getDateTime(size_t columnIndex) const = 0;
    [[nodiscard]] virtual string getBlob(size_t columnIndex) const = 0;
};

#endif //WINTER_RESULTSET_H
