//
// Created by Alienson on 11.2.2024..
//

#ifndef WINTER_JSONDESERIALIZER_H
#define WINTER_JSONDESERIALIZER_H

#include <string>

#include "../reflect/Reflect.h"
#include "../log/Logger.h"
#include "FieldTypeEnums.h"
#include "../util/stringUtils.h"
#include <cstdlib>

using namespace std;

class JsonDeserializer {
public:
    Reflect* deserialize(string& s, Reflect* response);

private:
    void setFieldValue(string& fieldValue, FieldType fieldType, Reflect* obj, Field* f, string& typeStr);
    void setFieldValueArray(string& fieldValue, FieldType fieldType, FieldType subType, Reflect* obj, Field* f);

    template<typename U>
    void insertVectorData(vector<U> *dest, string& str, U (*f)(string& val)){
        vector<string>* vec = StringUtils::splitArray(str);
        for(string& s : *vec){
            U u = f(s);
            dest->push_back(u);
        }
    }

    template<typename U>
    void parseArrayData(string& str, U (*f)(string& val), U* array, unsigned int* size){
        vector<string>* vec = StringUtils::splitArray(str, ','); //TODO handle , in strings
        array = (U*)calloc(vec->size(), sizeof(U));
        *size = vec->size();
        for(int i=0; i<vec->size(); i++){
            U u = f((*vec)[i]);
            array[i] = u;
        }
    }

    Reflect* tempObj;
};


#endif //WINTER_JSONDESERIALIZER_H
