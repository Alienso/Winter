//
// Created by Alienson on 6.2.2024..
//

#ifndef WINTER_FIELD_H
#define WINTER_FIELD_H

#include <string>
#include "../serialize/FieldEnums.h"

class Field {
public:

    Field(std::string& _name, std::string& _typeStr, int _type, std::string& _className, int _offset) :
    name(_name), typeStr(_typeStr), type(static_cast<FieldType>(_type)), className(_className), offset(_offset){}

    Field(const char* _name, const char* _typeStr, int _type, const char* _className, int _offset) :
            name(_name), typeStr(_typeStr), type(static_cast<FieldType>(_type)), className(_className), offset(_offset){}

    Field() : offset(0), type(FIELD_TYPE_INT) {}

    void* getAddress(void* object) const;

    void setInt(void* object, int value);
    void setLong(void* object, long value);
    void setChar(void* object, char value);
    void setFloat(void* object, float value);
    void setDouble(void* object, double value);
    void setShort(void* object, short value);
    void setString(void* object, std::string& value);
    void setString(void* object, const char* value);
    void setPtr(void* object, void* value);
    void setValue(void* object, void* data, unsigned int size);

    std::string name;
    std::string typeStr;
    FieldType type;
    std::string className;
    int offset;

    static Field INVALID;
};


#endif //WINTER_FIELD_H
