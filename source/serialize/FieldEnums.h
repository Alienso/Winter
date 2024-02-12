//
// Created by Alienson on 12.2.2024..
//

#ifndef WINTER_FIELDENUMS_H
#define WINTER_FIELDENUMS_H


#include "cstring"

enum JsonFieldType{
    JSON_FILED_TYPE_NATURAL_NUMBER,JSON_FILED_TYPE_REAL_NUMBER,JSON_FILED_TYPE_STRING,JSON_FILED_TYPE_OBJ
};

enum FieldType{
    FIELD_TYPE_SHORT,FIELD_TYPE_INT,FIELD_TYPE_LONG,FIELD_TYPE_FLOAT,FIELD_TYPE_DOUBLE,FIELD_TYPE_CHAR,FIELD_TYPE_STRING,FIELD_TYPE_OBJ,FIELD_TYPE_PTR
};

inline FieldType convertToFieldType(const char* s){
    if (strcmp(s, "int") == 0){
        return FIELD_TYPE_INT;
    }
    if (strcmp(s, "float") == 0){
        return FIELD_TYPE_FLOAT;
    }
    if (strcmp(s, "double") == 0){
        return FIELD_TYPE_DOUBLE;
    }
    if (strcmp(s, "char") == 0){
        return FIELD_TYPE_CHAR;
    }
    if (strcmp(s, "long") == 0){
        return FIELD_TYPE_LONG;
    }
    if (strcmp(s, "short") == 0){
        return FIELD_TYPE_SHORT;
    }
    return FIELD_TYPE_OBJ;
}

#endif //WINTER_FIELDENUMS_H
