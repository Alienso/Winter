//
// Created by Alienson on 11.2.2024..
//

#ifndef WINTER_JSONDESERIALIZER_H
#define WINTER_JSONDESERIALIZER_H

/**
 * Deserializes a json to a provided class of type Reflect
 * It is recommended to use pointers instead of objects for inner levels of deserialization since currently only a shallow
 * copy is made when copying the data. This is not an issue when pointers are made since function deserialize takes a
 * Reflect* pointer(to which the data is being written to), so no copying is required. Also, pointers can be null, objects
 * can not.
 */

#include <string>

#include "Reflect.h"
#include "Logger.h"
#include "FieldTypeUtil.h"
#include "stringUtils.h"
#include <cstdlib>

using namespace std;

class JsonDeserializer {
public:
    Reflect* deserialize(const string& s, Reflect* response);

private:
    void setObjectValue(Field* f, const string& fieldValue, Reflect* response);
    void setFieldValueArray(const string& fieldValue, FieldType fieldType, Reflect* obj, const Field* f);

    void insertVectorData(const string& source, vector<std::byte> *dest, const string &typeStr);
    void insertVectorPtrData(const string& source, vector<Reflect*> *dest, const string &typeStr);

    template<typename U>
    void insertVectorData(const string& source, U (*parseFunc)(string& val), vector<U> *dest) const{
        vector<string>* vec = StringUtils::splitArray(source);
        for (string& s : *vec)
            s = StringUtils::trim(s);
        for(string& s : *vec){
            U u = parseFunc(s);
            dest->push_back(u);
        }
    }

    template<typename U>
    void insertVectorPtrData(const string& source, U (*parseFunc)(string& val), vector<U*> *dest) const{
        vector<string>* vec = StringUtils::splitArray(source);
        for (string& s : *vec)
            s = StringUtils::trim(s);
        for(string& s : *vec){
            U* u = new U();
            *u = parseFunc(s);
            dest->push_back(u);
        }
    }

    template<typename U>
    void parseArrayData(const string& source, U (*parseFunc)(string& val), U* dest, unsigned int* destSize) const{
        vector<string>* vec = StringUtils::splitArray(source, ','); //TODO handle , in strings
        dest = (U*)calloc(vec->size(), sizeof(U));
        *destSize = vec->size();
        for(size_t i=0; i<vec->size(); i++){
            U u = parseFunc((*vec)[i]);
            dest[i] = u;
        }
    }

    template<typename T>
    void handleVecData(const Field* f, Reflect* obj, const bool isElemPtr, const string& fieldValue, T (*transformFunc)(string&)){
        if (!f->isPtr){
            if (!isElemPtr){
                auto* data = static_cast<vector<T>*>(f->getAddress(obj));
                insertVectorData<T>(fieldValue, transformFunc, data);
            }else{
                auto* data = static_cast<vector<T*>*>(f->getAddress(obj));
                insertVectorPtrData<T>(fieldValue, transformFunc, data);
            }
        }else{
            if (!isElemPtr){
                auto* data = new vector<T>(); //static_cast<vector<T>*>(*(f->getPtr(obj)));
                insertVectorData<T>(fieldValue, transformFunc, data);
                f->setPtr(obj, data);
            }else{
                auto* data = new vector<T*>(); //static_cast<vector<T*>*>(*(f->getPtr(obj)));
                insertVectorPtrData<T>(fieldValue, transformFunc, data);
                f->setPtr(obj, data);
            }
        }
    }
};


#endif //WINTER_JSONDESERIALIZER_H
