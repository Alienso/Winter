//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_STATEMENT_H
#define WINTER_STATEMENT_H

#include <Reflect.h>
#include "ResultSet.h"

#include <memory>

using namespace std;

class Statement {
public:

    Statement() = default;
    Statement(const char* s) : query(s){}
    virtual ~Statement() = default;

    virtual shared_ptr<ResultSet> execute();
    virtual shared_ptr<ResultSet> executeQuery(const char* s) = 0;
    virtual int executeUpdate(const char* s) = 0;

    virtual void setQueryTimeout(int milliseconds) = 0;
    virtual void close() = 0;

    Statement* setInt(int x, const char* name);
    Statement* setLong(long x, const char* name);
    Statement* setFloat(float x, const char* name);
    Statement* setDouble(double x, const char* name);
    Statement* setString(string& s, const char* name);
    Statement* setString(const char* s, const char* name);
    Statement* setObject(int o, const char* name);
    Statement* setNull(const char* name);
    Statement* setBool(bool b, const char* name);
    Statement* setByte(std::byte c, const char* name);
    Statement* setShort(short s, const char* name);

    Statement* setDate(string& s, const char* name);
    Statement* setDate(const char* s, const char* name);
    Statement* setTime(string& s, const char* name);
    Statement* setTime(const char* s, const char* name);
    Statement* setDateTime(string& s, const char* name);
    Statement* setDateTime(const char* s, const char* name);

    Statement* setBlob(string& s, const char* name);
    Statement* setBlob(const char* s, const char* name);

protected:
    string query;
    unordered_map<string,string> paramsMap{};
    size_t totalParamsLength = 0;

    void generateParameterMap();
    string buildQuery();
};


#endif //WINTER_STATEMENT_H
