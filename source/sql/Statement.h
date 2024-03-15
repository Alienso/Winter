//
// Created by Alienson on 14.3.2024..
//

#ifndef WINTER_STATEMENT_H
#define WINTER_STATEMENT_H

#include <string>

using namespace std;

class Statement {
public:
    virtual void executeQuery(const char* s) = 0;
    virtual void executeQuery(string& s) = 0;

    virtual int executeUpdate(const char* s) = 0;
    virtual int executeUpdate(string& s) = 0;

    virtual void setQueryTimeout(int seconds) = 0;
    virtual void close() = 0;
};

class PreparedStatement : public Statement{
public:
    virtual PreparedStatement* setInt(int x, int pos) = 0;
    virtual PreparedStatement* setLong(long x, int pos) = 0;
    virtual PreparedStatement* setFloat(float x, int pos) = 0;
    virtual PreparedStatement* setDouble(double x, int pos) = 0;
    virtual PreparedStatement* setString(string& s, int pos) = 0;
    virtual PreparedStatement* setString(const char* s, int pos) = 0;
    virtual PreparedStatement* setObject(int o, int pos) = 0;
    virtual PreparedStatement* setNull(int pos) = 0;
    virtual PreparedStatement* setBool(bool b, int pos) = 0;
    virtual PreparedStatement* setByte(byte c, int pos) = 0;
    virtual PreparedStatement* setShort(short s, int pos) = 0;

    virtual PreparedStatement* setDate(string& s, int pos) = 0;
    virtual PreparedStatement* setDate(const char* s, int pos) = 0;
    virtual PreparedStatement* setTime(string& s, int pos) = 0;
    virtual PreparedStatement* setTime(const char* s, int pos) = 0;
    virtual PreparedStatement* setDateTime(string& s, int pos) = 0;
    virtual PreparedStatement* setDateTime(const char* s, int pos) = 0;

    virtual PreparedStatement* setBlob(string& s, int pos) = 0;
    virtual PreparedStatement* setBlob(const char* s, int pos) = 0;
};


#endif //WINTER_STATEMENT_H
