//
// Created by Alienson on 6.2.2024..
//

#ifndef WINTER_FIELD_H
#define WINTER_FIELD_H

#include <string>
#include "FieldTypeEnums.h"

class Reflect;

class Field {
public:
    Field() : type(FIELD_TYPE_INT), offset(0) {}
    Field(std::string& _name, std::string& _typeStr, int _type, std::string& _className, int _offset) :
    name(_name), typeStr(_typeStr), type(static_cast<FieldType>(_type)), className(_className), offset(_offset){}

    Field(const char* _name, const char* _typeStr, int _type, const char* _className, int _offset) :
            name(_name), typeStr(_typeStr), type(static_cast<FieldType>(_type)), className(_className), offset(_offset){}

    void* getAddress(void* object) const;

    void set(void* object, const char* value) const;
    void setInt(void* object, int value) const;
    void setLong(void* object, long value) const;
    void setChar(void* object, char value) const;
    void setFloat(void* object, float value) const;
    void setDouble(void* object, double value) const;
    void setShort(void* object, short value) const;
    void setBool(void* object, bool value) const;
    void setByte(void* object, byte value) const;
    void setString(void* object, const std::string& value) const;
    void setString(void* object, const char* value) const;
    void setPtr(void* object, const void* value) const;
    void setValue(void* object, const void* data, unsigned int size) const;

    [[nodiscard]] int getInt(void* object) const;
    [[nodiscard]] long getLong(void* object) const;
    [[nodiscard]] char getChar(void* object) const;
    [[nodiscard]] float getFloat(void* object) const;
    [[nodiscard]] double getDouble(void* object) const;
    [[nodiscard]] short getShort(void* object) const;
    [[nodiscard]] bool getBool(void* object) const;
    [[nodiscard]] byte getByte(void* object) const;
    [[nodiscard]] string getString(void* object) const;
    [[nodiscard]] void** getPtr(void *object) const;

    static void copyValue(Reflect *source, Field &sourceField, Reflect *dest, Field &destField);

    std::string name;
    std::string typeStr;
    FieldType type;
    std::string className;
    int offset;

    static Field INVALID;
};


#endif //WINTER_FIELD_H
