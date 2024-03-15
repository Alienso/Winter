//
// Created by Alienson on 11.2.2024..
//

#ifndef WINTER_JSONDESERIALIZER_H
#define WINTER_JSONDESERIALIZER_H

#include <string>

#include "Reflect.h"
#include "Logger.h"
#include "FieldTypeEnums.h"
#include "stringUtils.h"
#include <cstdlib>

using namespace std;

class JsonDeserializer {
public:
    [[nodiscard]] Reflect* deserialize(const string& s, Reflect* response);

private:
    void setFieldValue(const string& fieldValue, FieldType fieldType, Reflect* obj, const Field* f, const string& typeStr);
    void setFieldValueArray(const string& fieldValue, FieldType fieldType, FieldType subType, Reflect* obj, const Field* f);

    template<typename U>
    void insertVectorData(const string& source, U (*parseFunc)(string& val), vector<U> *dest) const{
        vector<string>* vec = StringUtils::splitArray(source);
        for(string& s : *vec){
            U u = parseFunc(s);
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

    Reflect* tempObj;
};


#endif //WINTER_JSONDESERIALIZER_H
