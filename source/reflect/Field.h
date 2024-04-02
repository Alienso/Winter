//
// Created by Alienson on 6.2.2024..
//

#ifndef WINTER_FIELD_H
#define WINTER_FIELD_H

#include <string>
#include "FieldTypeUtil.h"
#include "Logger.h"

enum CopyType{
    COPY_TYPE_SHALLOW,
    COPY_TYPE_DEEP
};

class Reflect;

class Field {
public:
    Field() : type(FIELD_TYPE_INT), offset(0), isPtr(false), isVec(false) {}

    Field(const char* _name, const char* _typeStr, int _type, const char* _className, int _offset, bool _isPtr, bool _isVec) :
            name(_name), typeStr(_typeStr), type(static_cast<FieldType>(_type)), className(_className), offset(_offset), isPtr(_isPtr), isVec(_isVec){}

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

    static void copyValue(Reflect *source, const Field &sourceField, Reflect *dest, const Field &destField, CopyType copyType = COPY_TYPE_DEEP);
    static void copyObject(Reflect *source, Reflect *dest, CopyType copyType = COPY_TYPE_DEEP);


    std::string name;
    std::string typeStr;
    FieldType type;
    std::string className;
    int offset;
    bool isPtr;
    bool isVec;

    static Field INVALID;

private:
    typedef struct copyDetails{
        Reflect *source;
        const Field &sourceField;
        Reflect *dest;
        const Field &destField;
        CopyType copyType;

        copyDetails(Reflect *source_, const Field &sourceField_, Reflect *dest_, const Field &destField_, CopyType copyType_) :
        source(source_), sourceField(sourceField_), dest(dest_), destField(destField_), copyType(copyType_) {}
    } copyDetails;

    template<typename T>
    static void copyValue(copyDetails cp, T (Field::*getFunc)(void*) const, void (Field::*setFunc)(void*, T) const){
        T copyValue;
        if (cp.sourceField.isPtr)
            copyValue = **((T **) cp.sourceField.getPtr(cp.source));
        else
            copyValue = ((cp.sourceField).*getFunc)(cp.source);

        if (cp.destField.isPtr) {
            if (cp.copyType == COPY_TYPE_DEEP){
                if (cp.sourceField.isPtr) {
                    cp.destField.setPtr(cp.dest, *((T **) cp.sourceField.getPtr(cp.source)));
                }else{
                    wtLogError("Can not make DEEP copy of non pointer! FieldName: %s", cp.sourceField.name.data());
                }
            }else {
                auto *valuePtr = new T();
                *valuePtr = copyValue;
                cp.destField.setPtr(cp.dest, valuePtr);
            }
        }
        else {
            ((cp.destField).*setFunc)(cp.dest, copyValue);
        }
    }

    template<typename T>
    void setValueInternal(T t, void* obj, void (Field::*setFunc)(void*, T) const) const{
        if (this->isPtr){
            auto *valuePtr = new T();
            *valuePtr = t;
            this->setPtr(obj, valuePtr);
        }
        else{
            (this->*setFunc)(obj, t);
        }
    }

};


#endif //WINTER_FIELD_H
