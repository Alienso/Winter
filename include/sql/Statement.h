//
// Created by Alienson on 14.3.2024.
//

#ifndef WINTER_STATEMENT_H
#define WINTER_STATEMENT_H

#include "reflect/Reflect.h"
#include "sql/ResultSet.h"

#include <memory>


class Statement {
public:

    Statement() = default;
    Statement(const char* s) : query(s){}
    virtual ~Statement() = default;

    virtual std::shared_ptr<ResultSet> execute();
    virtual std::shared_ptr<ResultSet> executeQuery(const char* s) = 0;
    virtual int executeUpdate(const char* s) = 0;

    virtual void setQueryTimeout(int milliseconds) = 0;
    virtual void close() = 0;

    Statement* setInt(int x, const char* name);
    Statement* setLong(long x, const char* name);
    Statement* setFloat(float x, const char* name);
    Statement* setDouble(double x, const char* name);
    Statement* setString(std::string& s, const char* name);
    Statement* setString(const char* s, const char* name);
    Statement* setObject(int o, const char* name);
    Statement* setNull(const char* name);
    Statement* setBool(bool b, const char* name);
    Statement* setByte(std::byte c, const char* name);
    Statement* setShort(short s, const char* name);

    Statement* setDate(std::string& s, const char* name);
    Statement* setDate(const char* s, const char* name);
    Statement* setTime(std::string& s, const char* name);
    Statement* setTime(const char* s, const char* name);
    Statement* setDateTime(std::string& s, const char* name);
    Statement* setDateTime(const char* s, const char* name);

    Statement* setBlob(std::string& s, const char* name);
    Statement* setBlob(const char* s, const char* name);

protected:
    std::string query;
    std::unordered_map<std::string,std::string> paramsMap{};
    size_t totalParamsLength = 0;

    void generateParameterMap();
    std::string buildQuery();
};


#endif //WINTER_STATEMENT_H
