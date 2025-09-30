//
// Created by Alienson on 12.2.2024.
//

#ifndef WINTER_FIELDTYPEUTIL_H
#define WINTER_FIELDTYPEUTIL_H

#include "util/stringUtils.h"

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
    FIELD_TYPE_BOOL,
    FIELD_TYPE_BYTE, // 7
    FIELD_TYPE_STRING,
    FIELD_TYPE_VECTOR,
    FIELD_TYPE_OBJ,
    FIELD_TYPE_ARRAY,//11
};

//TODO have map for this?
[[nodiscard]] inline FieldType convertToFieldType(const std::string& s){
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
    if (StringUtils::startsWith(s, "std::vector") || StringUtils::startsWith(s, "vector"))
        return FIELD_TYPE_VECTOR;
    if (StringUtils::startsWith(s, "std::string") || StringUtils::startsWith(s, "string"))
        return FIELD_TYPE_STRING;
    return FIELD_TYPE_OBJ;
}


inline void getArraySubType(const std::string& s, std::string& type, bool* isPtr){
    size_t index;

    //array check
    /*index = s.find_last_of(']');
    if (index != string::npos){
        index = s.find(' ');
        type = s.substr(0,index-1);
        *isPtr = false;
        return;
    }*/

    //vector check
    index = s.find('<');
    if (index != std::string::npos) {
        size_t endIndex = s.find('>', index);
        if (s[endIndex - 1] == '*'){
            type = s.substr(index + 1,endIndex - index - 2);
            *isPtr = true;
            return;
        }else{
            type = s.substr(index + 1,endIndex - index - 1);
            *isPtr = false;
            return;
        }
    }

    std::cout << "Error! Unknown ArraySubType: " << s << '\n';
    type = "int";
    *isPtr = false;
}

[[nodiscard]] inline FieldType getArraySubType(const std::string& s){
    std::string type;
    bool isPtr;
    getArraySubType(s, type, &isPtr);
    return convertToFieldType(type);
}

[[nodiscard]] inline JsonFieldType getJsonFieldType(const std::string& s) {
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
            if (s.find('.') != std::string::npos)
                return JSON_FILED_TYPE_REAL_NUMBER;
            return JSON_FILED_TYPE_NATURAL_NUMBER;
    }
}

[[nodiscard]] inline JsonFieldType getJsonFieldSubType(const std::string& s) {
    size_t i = 1;
    while (i < s.size() && isspace(s[i])) i++;

    switch(s[i]){
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
            if (s.find('.') != std::string::npos)
                return JSON_FILED_TYPE_REAL_NUMBER;
            return JSON_FILED_TYPE_NATURAL_NUMBER;
    }
}

[[nodiscard]] inline bool areTypesCompatible(const JsonFieldType jsonType, const FieldType fieldType) {
    switch (jsonType) {
        case JSON_FILED_TYPE_NATURAL_NUMBER:
            if (fieldType == FIELD_TYPE_SHORT || fieldType == FIELD_TYPE_INT || fieldType == FIELD_TYPE_LONG ||
            fieldType == FIELD_TYPE_FLOAT || fieldType == FIELD_TYPE_DOUBLE) // natural number is a valid float/double
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
            if (fieldType == FIELD_TYPE_OBJ)
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

#endif //WINTER_FIELDTYPEUTIL_H
