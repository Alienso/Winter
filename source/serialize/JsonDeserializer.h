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

using namespace std;

template<typename T>
class JsonDeserializer {
public:
    Reflect* deserialize (string& s){
        Reflect* response = new T();
        string fieldName, fieldValue;
        JsonFieldType fieldType;

        int offset = 1;
        int semicolonIndex;
        int commaIndex;

        while(true){

            semicolonIndex = s.find(':', offset);
            if (semicolonIndex == string::npos)
                return response;

            commaIndex = s.find(',', semicolonIndex);
            if (commaIndex == string::npos)
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
                setFieldValue(fieldName, fieldValue, expectedType, response, f);
            }else{
                //wtLogError("Incompatible types: %d and %d for field %s", fieldType, expectedType, fieldName.data());
                std::cerr << "Incompatible types: " << fieldType << " and " << expectedType << " for field: " << fieldName;
            }
        }
    }


private:
    void setFieldValue(string& fieldName, string& fieldValue, FieldType fieldType, Reflect* obj, Field* f){
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

    JsonFieldType deduceType(string& s) {
        switch(s[0]){
            case '"':
                return JSON_FILED_TYPE_STRING;
                break;
            case '{':
                return JSON_FILED_TYPE_OBJ;
                break;
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
            default:
                return false;
        }
    }

};


#endif //WINTER_JSONDESERIALIZER_H
