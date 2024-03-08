//
// Created by Alienson on 6.2.2024..
//

#ifndef WINTER_FIELD_H
#define WINTER_FIELD_H

#include <string>
#include "../serialize/FieldTypeEnums.h"

class Field {
public:

    Field(std::string& _name, std::string& _typeStr, int _type, std::string& _className, int _offset) :
    name(_name), typeStr(_typeStr), type(static_cast<FieldType>(_type)), className(_className), offset(_offset){}

    Field(const char* _name, const char* _typeStr, int _type, const char* _className, int _offset) :
            name(_name), typeStr(_typeStr), type(static_cast<FieldType>(_type)), className(_className), offset(_offset){}

    Field() : offset(0), type(FIELD_TYPE_INT) {}

    void* getAddress(void* object) const;

    void setInt(void* object, int value) const;
    void setLong(void* object, long value) const;
    void setChar(void* object, char value) const;
    void setFloat(void* object, float value) const;
    void setDouble(void* object, double value) const;
    void setShort(void* object, short value) const;
    void setString(void* object, std::string& value) const;
    void setString(void* object, const char* value) const;
    void setPtr(void* object, void* value) const;
    void setValue(void* object, void* data, unsigned int size) const;

    int getInt(void* object) const;
    long getLong(void* object) const;
    char getChar(void* object) const;
    float getFloat(void* object) const;
    double getDouble(void* object) const;
    short getShort(void* object) const;
    string getString(void* object) const;
    void** getPtr(void *object) const;

    std::string name;
    std::string typeStr;
    FieldType type;
    std::string className;
    int offset;

    static Field INVALID;
};


#endif //WINTER_FIELD_H
