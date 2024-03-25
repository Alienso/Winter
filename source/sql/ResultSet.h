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
    virtual Reflect* getResult() = 0;
    virtual vector<Reflect*>* getResultList() = 0;
    virtual bool next() = 0;

protected:
    virtual int getInt(int columnIndex) = 0;
    virtual long getLong(int columnIndex) = 0;
    virtual float getFloat(int columnIndex) = 0;
    virtual double getDouble(int columnIndex) = 0;
    virtual string getString(int columnIndex) = 0;
    virtual bool getBool(int columnIndex) = 0;
    virtual byte getByte(int columnIndex) = 0;
    virtual short getShort(int columnIndex) = 0;
    virtual string getDate(int columnIndex) = 0;
    virtual string getTime(int columnIndex) = 0;
    virtual string getDateTime(int columnIndex) = 0;
    virtual string getBlob(int columnIndex) = 0;
};

#endif //WINTER_RESULTSET_H
