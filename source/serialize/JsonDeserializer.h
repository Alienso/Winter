//
// Created by Alienson on 11.2.2024..
//

#ifndef WINTER_JSONDESERIALIZER_H
#define WINTER_JSONDESERIALIZER_H

#include <string>

#include "../reflect/Reflect.h"
#include "../log/Logger.h"
#include "FieldEnums.h"
#include "../util/stringUtils.h"
#include <cstdlib>

using namespace std;

class JsonDeserializer {
public:
    Reflect* deserialize(string& s, Reflect* response){
        //Reflect* response = new T(); //TODO T is used only here, maybe make this func take second parameter Reflect* so this is not a template
        string fieldName, fieldValue;
        JsonFieldType fieldType;

        size_t offset = 1;
        size_t semicolonIndex;
        size_t commaIndex;

        int bracketCounter = 0;
        bool inStr = false;

        while(true){

            semicolonIndex = s.find(':', offset);
            if (semicolonIndex == string::npos)
                return response;

            commaIndex = string::npos;
            for (size_t i=semicolonIndex; i<s.size(); i++){
                if (s[i] == '[' || s[i] == '{')
                    bracketCounter+=s[i];
                if (s[i] == ']' || s[i] == '}')
                    bracketCounter-=s[i]+2;
                if (s[i] == '"' && s[i-1] != '\\')
                    inStr = !inStr;
                if (bracketCounter == 0 && !inStr && s[i] == ',') {
                    commaIndex = i;
                    break;
                }
            }

            if (commaIndex == string::npos || commaIndex >= s.size())
                commaIndex = s.size() - 1;

            fieldName = s.substr(offset, semicolonIndex - offset);
            fieldValue = s.substr(semicolonIndex + 1, commaIndex - semicolonIndex - 1);

            fieldName = StringUtils::trim(fieldName);
            fieldName = fieldName.substr(1,fieldName.size() - 2); //remove ""
            fieldValue = StringUtils::trim(fieldValue);

            offset = commaIndex + 1;

            fieldType = deduceType(fieldValue);
            Field* f = Reflect::getField(fieldName.data());
            if (f == &Field::INVALID){
                continue;
            }
            FieldType expectedType = f->type;

            if (areTypesCompatible(fieldType, expectedType)) {
                if (fieldType == JSON_FIELD_TYPE_ARRAY) {
                    FieldType subType = getArraySubType(f->typeStr);
                    setFieldValueArray(fieldValue, expectedType, subType, response, f);
                }
                else setFieldValue(fieldValue, expectedType, response, f);
            }else{
                //wtLogError("Incompatible types: %d and %d for field %s", fieldType, expectedType, fieldName.data());
                std::cerr << "Incompatible types: " << fieldType << " and " << expectedType << " for field: " << fieldName;
            }
        }
    }


private:
    void setFieldValue(string& fieldValue, FieldType fieldType, Reflect* obj, Field* f){
        switch(fieldType){
            case FIELD_TYPE_INT:
                f->setInt(obj, stoi(fieldValue));
                break;
            case FIELD_TYPE_SHORT:
                f->setShort(obj, (short)stoi(fieldValue));
                break;
            case FIELD_TYPE_LONG:
                f->setLong(obj, stol(fieldValue));
                break;
            case FIELD_TYPE_CHAR:
                f->setChar(obj, fieldValue[0]);
                break;
            case FIELD_TYPE_FLOAT:
                f->setFloat(obj, stof(fieldValue));
                break;
            case FIELD_TYPE_DOUBLE:
                f->setDouble(obj, stod(fieldValue));
                break;
            case FIELD_TYPE_STRING:
                f->setString(obj, fieldValue);
                break;
            case FIELD_TYPE_OBJ:
                //serialize field
                f->setValue(obj, &fieldValue, sizeof(fieldValue));
            default:
                f->setInt(obj, 0);
        }
    }

    void setFieldValueArray(string& fieldValue, FieldType fieldType, FieldType subType, Reflect* obj, Field* f){
        switch(subType){
            case FIELD_TYPE_INT:
                if (fieldType == FIELD_TYPE_VECTOR){
                    //std::vector<int> vec;
                    //f->setValue(obj, &vec, sizeof(vec));
                    vector<int>* data = static_cast<vector<int> *>(f->getAddress(obj));
                    insertVectorData<int>(data, fieldValue, [](string &val){ return stoi(val);});
                }else {
                    int *array = nullptr;
                    unsigned int size;
                    parseArrayData<int>(fieldValue, [](string &val){ return stoi(val);}, array, &size);
                    f->setValue(obj, array, size * sizeof(int));
                }
                break;
            case FIELD_TYPE_SHORT:
                f->setShort(obj, (short)stoi(fieldValue));
                break;
            case FIELD_TYPE_LONG:
                f->setLong(obj, stol(fieldValue));
                break;
            case FIELD_TYPE_CHAR:
                f->setChar(obj, fieldValue[0]);
                break;
            case FIELD_TYPE_FLOAT:
                f->setFloat(obj, stof(fieldValue));
                break;
            case FIELD_TYPE_DOUBLE:
                f->setDouble(obj, stod(fieldValue));
                break;
            case FIELD_TYPE_STRING:
                f->setString(obj, fieldValue);
                break;
            case FIELD_TYPE_OBJ:
                //serialize field
                f->setValue(obj, &fieldValue, sizeof(fieldValue));
            default:
                f->setInt(obj, 0);
        }
    }

    JsonFieldType deduceType(string& s) {
        switch(s[0]){
            case '"':
                return JSON_FILED_TYPE_STRING;
            case '{':
                return JSON_FILED_TYPE_OBJ;
            case '[':
                return JSON_FIELD_TYPE_ARRAY;
            default:
                if (s.find('.') != string::npos)
                    return JSON_FILED_TYPE_REAL_NUMBER;
                return JSON_FILED_TYPE_NATURAL_NUMBER;
        }
    }

    bool areTypesCompatible(JsonFieldType jsonType, FieldType fieldType) {
        switch (jsonType) {
            case JSON_FILED_TYPE_NATURAL_NUMBER:
                if (fieldType == FIELD_TYPE_SHORT || fieldType == FIELD_TYPE_INT || fieldType == FIELD_TYPE_LONG)
                    return true;
                return false;
            case JSON_FILED_TYPE_REAL_NUMBER:
                if (fieldType == FIELD_TYPE_FLOAT || fieldType == FIELD_TYPE_DOUBLE)
                    return true;
                return false;
            case JSON_FILED_TYPE_STRING:
                if (fieldType == FIELD_TYPE_STRING)
                    return true;
            case JSON_FILED_TYPE_OBJ:
                if (fieldType == FIELD_TYPE_OBJ)
                    return true;
                return false;
            case JSON_FIELD_TYPE_ARRAY:
                if (fieldType == FIELD_TYPE_ARRAY || fieldType == FIELD_TYPE_VECTOR)
                    return true;
                return false;
            default:
                return false;
        }
    }

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

};


#endif //WINTER_JSONDESERIALIZER_H
