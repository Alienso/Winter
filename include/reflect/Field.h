//
// Created by Alienson on 6.2.2024.
//

#ifndef WINTER_FIELD_H
#define WINTER_FIELD_H

#include <string>
#include "serialize/FieldTypeUtil.h"
#include "log/Loggy.h"

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
    std::string getAsString(Reflect* object, char stringChar = 0) const;

    void set(void* object, const char* value) const;
    void setInt(void* object, int value) const;
    void setLong(void* object, long value) const;
    void setChar(void* object, char value) const;
    void setFloat(void* object, float value) const;
    void setDouble(void* object, double value) const;
    void setShort(void* object, short value) const;
    void setBool(void* object, bool value) const;
    void setByte(void* object, std::byte value) const;
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
    [[nodiscard]] std::byte getByte(void* object) const;
    [[nodiscard]] std::string getString(void* object) const;
    [[nodiscard]] void** getPtr(void *object) const;

    static void copyValue(Reflect *source, const Field &sourceField, Reflect *dest, const Field &destField, CopyType copyType = COPY_TYPE_DEEP);
    static void copyVectorValue(Reflect *source, const Field &sourceField, Reflect *dest, const Field &destField, CopyType copyType = COPY_TYPE_DEEP);
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
    typedef struct CopyDetails{
        Reflect *source;
        const Field &sourceField;
        Reflect *dest;
        const Field &destField;
        CopyType copyType;

        CopyDetails(Reflect *source_, const Field &sourceField_, Reflect *dest_, const Field &destField_, CopyType copyType_) :
        source(source_), sourceField(sourceField_), dest(dest_), destField(destField_), copyType(copyType_) {}
    } CopyDetails;

    template<typename T>
    static void copyValue(CopyDetails& cp, T (Field::*getFunc)(void*) const, void (Field::*setFunc)(void*, T) const){
        if (cp.destField.isPtr) {
            if (cp.copyType == COPY_TYPE_DEEP){
                if (cp.sourceField.isPtr) {
                    cp.destField.setPtr(cp.dest, *((T **) cp.sourceField.getPtr(cp.source)));
                }else{
                    wtLogError("Can not make DEEP copy of non pointer! FieldName: {}", cp.sourceField.name);
                }
            }else {
                auto *valuePtr = new T();
                if (cp.sourceField.isPtr)
                    *valuePtr = **((T **) cp.sourceField.getPtr(cp.source));
                else
                    *valuePtr = ((cp.sourceField).*getFunc)(cp.source);
                cp.destField.setPtr(cp.dest, valuePtr);
            }
        }
        else {
            if (cp.sourceField.isPtr)
                ((cp.destField).*setFunc)(cp.dest, **((T **) cp.sourceField.getPtr(cp.source)));
            else
                ((cp.destField).*setFunc)(cp.dest, ((cp.sourceField).*getFunc)(cp.source));

        }
    }

    static void handleVectorValueCopyObject(CopyDetails& copyDetails, bool isDestElemPtr, bool isSourceElemPtr){
        if (!isDestElemPtr) {
            wtLogError("Dest object must be a ptr");
            return;
        }
        if (isSourceElemPtr){
            copyVectorPtrValue<Reflect*>(copyDetails);
        } else {
            wtLogError("Copying from non pointer to pointer is currently not supported!");
        }

    }

    template<typename T>
    static void handleVectorValueCopy(CopyDetails& copyDetails, bool isDestElemPtr, bool isSourceElemPtr){
        //TODO add support for other types
        if (isDestElemPtr){
            if (isSourceElemPtr){
                copyVectorPtrValue<T>(copyDetails);
            } else {
                wtLogError("Copying from non pointer to pointer is currently not supported!");
            }
        }
        else {
            if (isSourceElemPtr){
                wtLogError("Copying from pointer to non pointer is currently not supported!");
            } else {
                copyVectorValue<T>(copyDetails);
            }
        }
    }

    static void copyVectorValueBool(CopyDetails& cp){
        std::vector<bool> *sourceVec;
        std::vector<bool> *destVec;
        if (cp.destField.isPtr)
            destVec = *((std::vector<bool> **) cp.destField.getPtr(cp.dest));
        else destVec = (std::vector<bool> *) cp.destField.getAddress(cp.dest);
        if (cp.sourceField.isPtr)
            sourceVec = *((std::vector<bool> **) cp.sourceField.getPtr(cp.source));
        else sourceVec = (std::vector<bool> *) cp.sourceField.getAddress(cp.source);

        if (cp.copyType == COPY_TYPE_DEEP && cp.destField.isPtr && !cp.sourceField.isPtr) {
            wtLogError("Can not make DEEP copy of non pointer! FieldName: {}", cp.sourceField.name);
        } else {
            destVec->reserve(sourceVec->size());
            for (bool val : *sourceVec)
                destVec->push_back(val);
        }
    }

    template<typename T>
    static void copyVectorValue(CopyDetails& cp){
        std::vector<T> *sourceVec;
        std::vector<T> *destVec;
        if (cp.destField.isPtr)
            destVec = *((std::vector<T> **) cp.destField.getPtr(cp.dest));
        else destVec = (std::vector<T> *) cp.destField.getAddress(cp.dest);
        if (cp.sourceField.isPtr)
            sourceVec = *((std::vector<T> **) cp.sourceField.getPtr(cp.source));
        else sourceVec = (std::vector<T> *) cp.sourceField.getAddress(cp.source);

        if (cp.copyType == COPY_TYPE_DEEP && cp.destField.isPtr && !cp.sourceField.isPtr) {
            wtLogError("Can not make DEEP copy of non pointer! FieldName: {}", cp.sourceField.name);
        } else {
            destVec->reserve(sourceVec->size());
            for (T& val : *sourceVec)
                destVec->push_back(val);
        }
    }

    template<typename T>
    static void copyVectorPtrValue(CopyDetails& cp){
        std::vector<T*> *sourceVec;
        std::vector<T*> *destVec;
        if (cp.destField.isPtr)
            destVec = *((std::vector<T*> **) cp.destField.getPtr(cp.dest));
        else destVec = (std::vector<T*> *) cp.destField.getAddress(cp.dest);
        if (cp.sourceField.isPtr)
            sourceVec = *((std::vector<T*> **) cp.sourceField.getPtr(cp.source));
        else sourceVec = (std::vector<T*> *) cp.sourceField.getAddress(cp.source);

        if (cp.copyType == COPY_TYPE_DEEP && cp.destField.isPtr && !cp.sourceField.isPtr) {
            wtLogError("Can not make DEEP copy of non pointer! FieldName: {}", cp.sourceField.name);
        } else {
            destVec->reserve(sourceVec->size());
            for (T* val : *sourceVec)
                destVec->push_back(val);
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

    template<typename T>
    std::string getAsString(Reflect* obj, T (Field::*getFunc)(void*) const, std::string (*to_string_func)(T)) const{
        T t;
        if (!isPtr) {
            t = (this->*getFunc)(obj);
            return to_string_func(t);
        }
        else{
            void* ptr = *getPtr(obj);
            if (ptr == nullptr)
                return "null";
            t = *(T*)ptr;
            return to_string_func(t);
        }
    }

};


#endif //WINTER_FIELD_H
