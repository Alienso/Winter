//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_STATEMENT_H
#define WINTER_STATEMENT_H

#include <string>
#include <../reflect/Reflect.h>
#include "ResultSet.h"

using namespace std;

class Statement {
public:

    Statement() = default;
    virtual ~Statement() = default;

    virtual ResultSet* executeQuery(const char* s) = 0;
    virtual int executeUpdate(const char* s) = 0;

    virtual void setQueryTimeout(int seconds) = 0;
    virtual void close() = 0;

    Statement* setInt(int x, int pos);
    Statement* setLong(long x, int pos);
    Statement* setFloat(float x, int pos);
    Statement* setDouble(double x, int pos);
    Statement* setString(string& s, int pos);
    Statement* setString(const char* s, int pos);
    Statement* setObject(int o, int pos);
    Statement* setNull(int pos);
    Statement* setBool(bool b, int pos);
    Statement* setByte(byte c, int pos);
    Statement* setShort(short s, int pos);

    Statement* setDate(string& s, int pos);
    Statement* setDate(const char* s, int pos);
    Statement* setTime(string& s, int pos);
    Statement* setTime(const char* s, int pos);
    Statement* setDateTime(string& s, int pos);
    Statement* setDateTime(const char* s, int pos);

    Statement* setBlob(string& s, int pos);
    Statement* setBlob(const char* s, int pos);

protected:
    string query;
};


#endif //WINTER_STATEMENT_H
