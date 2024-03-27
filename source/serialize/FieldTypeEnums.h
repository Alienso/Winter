//
// Created by Alienson on 12.2.2024..
//

#ifndef WINTER_FIELDTYPEENUMS_H
#define WINTER_FIELDTYPEENUMS_H

#include "stringUtils.h"

#include <cstring>
#include <iostream>

enum JsonFieldType{
    JSON_FILED_TYPE_NATURAL_NUMBER,
    JSON_FILED_TYPE_REAL_NUMBER,
    JSON_FILED_TYPE_STRING,
    JSON_FIELD_TYPE_ARRAY,
    JSON_FILED_TYPE_OBJ,
    JSON_FIELD_TYPE_BOOL
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
    FIELD_TYPE_VECTOR,
    FIELD_TYPE_BOOL, // 11
    FIELD_TYPE_BYTE
};

//TODO have map for this?
[[nodiscard]] inline FieldType convertToFieldType(const string& s){
    if (s == "int")
        return FIELD_TYPE_INT;
    if (s == "float")
        return FIELD_TYPE_FLOAT;
    if (s == "double")
        return FIELD_TYPE_DOUBLE;
    if (s == "char")
        return FIELD_TYPE_CHAR;
    if (s == "long")
        return FIELD_TYPE_LONG;
    if (s == "short")
        return FIELD_TYPE_SHORT;
    if (s == "bool")
        return FIELD_TYPE_BOOL;
    if (s == "byte")
        return FIELD_TYPE_BYTE;
    if (StringUtils::startsWith(s, "stdvec") || StringUtils::startsWith(s, "vec")) //TODO have single name
        return FIELD_TYPE_VECTOR;
    if (StringUtils::startsWith(s, "stdstring") || StringUtils::startsWith(s, "string"))
        return FIELD_TYPE_STRING;
    if (s.rfind('*') != string::npos)
        return FIELD_TYPE_PTR;
    return FIELD_TYPE_OBJ;
}

[[nodiscard]] inline FieldType getArraySubFieldType(const string& s){
    size_t index;

    //array check
    index = s.find_last_of(']');
    if (index != string::npos){
        index = s.find(' ');
        return convertToFieldType(s.substr(0,index-1));
    }

    //vector check
    index = s.find('<');
    if (index != string::npos) {
        size_t endIndex = s.find('>', index);
        return convertToFieldType(s.substr(index + 1,endIndex - index - 1));
    }

    std::cout << "Error! Unknown ArraySubType: " << s << '\n';
    return FIELD_TYPE_INT;
}

[[nodiscard]] inline string getArraySubType(const string& s){
    size_t index;

    //array check
    index = s.find_last_of(']');
    if (index != string::npos){
        index = s.find(' ');
        return s.substr(0,index-1);
    }

    //vector check
    index = s.find('<');
    if (index != string::npos) {
        size_t endIndex = s.find('>', index);
        return s.substr(index + 1,endIndex - index - 1);
    }

    std::cout << "Error! Unknown ArraySubType: " << s << '\n';
    return "int";
}

[[nodiscard]] inline JsonFieldType convertToJsonFieldType(const string& s) {
    switch(s[0]){
        case '"':
            return JSON_FILED_TYPE_STRING;
        case '{':
            return JSON_FILED_TYPE_OBJ;
        case '[':
            return JSON_FIELD_TYPE_ARRAY;
        case 't':
        case 'T':
        case 'f':
        case 'F':
            return JSON_FIELD_TYPE_BOOL;
        default:
            if (s.find('.') != string::npos)
                return JSON_FILED_TYPE_REAL_NUMBER;
            return JSON_FILED_TYPE_NATURAL_NUMBER;
    }
}

[[nodiscard]] inline bool areTypesCompatible(const JsonFieldType jsonType, const FieldType fieldType) {
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
        case JSON_FIELD_TYPE_BOOL:
            if (fieldType == FIELD_TYPE_BOOL)
                return true;
            return false;
        default:
            return false;
    }
}

#endif //WINTER_FIELDTYPEENUMS_H
