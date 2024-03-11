//
// Created by Alienson on 12.2.2024..
//

#ifndef WINTER_FIELDTYPEENUMS_H
#define WINTER_FIELDTYPEENUMS_H

#include "../util/stringUtils.h"

#include <cstring>
#include <iostream>

enum JsonFieldType{
    JSON_FILED_TYPE_NATURAL_NUMBER,
    JSON_FILED_TYPE_REAL_NUMBER,
    JSON_FILED_TYPE_STRING,
    JSON_FIELD_TYPE_ARRAY,
    JSON_FILED_TYPE_OBJ
};

enum FieldType{
    FIELD_TYPE_SHORT,
    FIELD_TYPE_INT,
    FIELD_TYPE_LONG,
    FIELD_TYPE_FLOAT, // 3
    FIELD_TYPE_DOUBLE,
    FIELD_TYPE_CHAR,
    FIELD_TYPE_STRING,
    FIELD_TYPE_OBJ, // 7
    FIELD_TYPE_PTR,
    FIELD_TYPE_ARRAY,
    FIELD_TYPE_VECTOR
};

[[nodiscard]] inline FieldType convertToFieldType(const char* s){
    if (strcmp(s, "int") == 0)
        return FIELD_TYPE_INT;
    if (strcmp(s, "float") == 0)
        return FIELD_TYPE_FLOAT;
    if (strcmp(s, "double") == 0)
        return FIELD_TYPE_DOUBLE;
    if (strcmp(s, "char") == 0)
        return FIELD_TYPE_CHAR;
    if (strcmp(s, "long") == 0)
        return FIELD_TYPE_LONG;
    if (strcmp(s, "short") == 0)
        return FIELD_TYPE_SHORT;
    if (StringUtils::startsWith(s, "stdvec") || StringUtils::startsWith(s, "vec")) //TODO have single name
        return FIELD_TYPE_VECTOR;
    if (StringUtils::startsWith(s, "stdstring") || StringUtils::startsWith(s, "string"))
        return FIELD_TYPE_STRING;
    return FIELD_TYPE_OBJ;
}

[[nodiscard]] inline FieldType getArraySubType(const string& s){
    size_t index;

    //array check
    index = s.find_last_of(']');
    if (index != string::npos){
        index = s.find(' ');
        return convertToFieldType(s.substr(0,index-1).data());
    }

    //vector check
    index = s.find('<');
    if (index != string::npos) {
        size_t endIndex = s.find('>', index);
        return convertToFieldType(s.substr(index + 1,endIndex - index - 1).data());
    }

    std::cout << "Error! Unknown ArraySubType: " << s << '\n';
    return FIELD_TYPE_INT;
}

[[nodiscard]] inline JsonFieldType convertToJsonFieldType(string& s) {
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

[[nodiscard]] inline bool areTypesCompatible(JsonFieldType jsonType, FieldType fieldType) {
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
            return false;
        case JSON_FILED_TYPE_OBJ:
            if (fieldType == FIELD_TYPE_OBJ || fieldType == FIELD_TYPE_PTR)
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

#endif //WINTER_FIELDTYPEENUMS_H
