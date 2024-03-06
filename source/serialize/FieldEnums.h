//
// Created by Alienson on 12.2.2024..
//

#ifndef WINTER_FIELDENUMS_H
#define WINTER_FIELDENUMS_H


#include "cstring"
#include "../util/stringUtils.h"

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

inline FieldType convertToFieldType(const char* s){
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
    if (StringUtils::startsWith(s, "stdvec") || StringUtils::startsWith(s, "vec"))
        return FIELD_TYPE_VECTOR;
    if (StringUtils::startsWith(s, "stdstring") || StringUtils::startsWith(s, "string"))
        return FIELD_TYPE_STRING;
    //TODO ARRAYS!
    return FIELD_TYPE_OBJ;
}

inline FieldType getArraySubType(string& s){
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

    return FIELD_TYPE_INT;
}

#endif //WINTER_FIELDENUMS_H
