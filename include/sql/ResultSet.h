//
// Created by Alienson on 14.3.2024.
//

#ifndef WINTER_RESULTSET_H
#define WINTER_RESULTSET_H

#include <string>
#include <memory>
#include "reflect/Reflect.h"

class ResultSet{
public:
    [[nodiscard]] virtual std::shared_ptr<Reflect> getResult(Reflect* (*allocator)()) const = 0;
    [[nodiscard]] virtual std::shared_ptr<std::vector<std::shared_ptr<Reflect>>> getResultList(Reflect* (*allocator)()) const = 0;
    virtual bool next() = 0;

    [[nodiscard]] virtual int getInt(size_t columnIndex) const = 0;
    [[nodiscard]] virtual long getLong(size_t columnIndex) const = 0;
    [[nodiscard]] virtual float getFloat(size_t columnIndex) const = 0;
    [[nodiscard]] virtual double getDouble(size_t columnIndex) const = 0;
    [[nodiscard]] virtual std::string getString(size_t columnIndex) const = 0;
    [[nodiscard]] virtual bool getBool(size_t columnIndex) const = 0;
    [[nodiscard]] virtual std::byte getByte(size_t columnIndex) const = 0;
    [[nodiscard]] virtual short getShort(size_t columnIndex) const = 0;
    [[nodiscard]] virtual std::string getDate(size_t columnIndex) const = 0;
    [[nodiscard]] virtual std::string getTime(size_t columnIndex) const = 0;
    [[nodiscard]] virtual std::string getDateTime(size_t columnIndex) const = 0;
    [[nodiscard]] virtual std::string getBlob(size_t columnIndex) const = 0;
};

#endif //WINTER_RESULTSET_H
