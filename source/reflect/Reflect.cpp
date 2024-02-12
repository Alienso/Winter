//
// Created by Alienson on 6.2.2024..
//

#include "Reflect.h"

std::vector<Field> Reflect::declaredFields = {};
std::vector<Method> Reflect::declaredMethods = {};

std::vector<Field> &Reflect::getDeclaredFields() {
    return declaredFields;
}

Field *Reflect::getField(const char *fieldName) {
    for (Field& f : declaredFields){
        if (f.name == fieldName)
            return &f;
    }
    return &Field::INVALID;
}

std::vector<Method> &Reflect::getDeclaredMethods() {
    return declaredMethods;
}

Method &Reflect::getMethod(const char *methodName) {
    return declaredMethods[0];
}
