//
// Created by Alienson on 6.2.2024..
//

#include "Reflect.h"

#include "../log/Logger.h"

std::vector<Field> &Reflect::getDeclaredFields() {
    wtLogError("getDeclaredFields called for Reflect*!");
    return declaredFields;
}

Field *Reflect::getField(const char *fieldName) {
    wtLogError("getField called for Reflect*!");
    return &Field::INVALID;
}

std::vector<Method> &Reflect::getDeclaredMethods() {
    return declaredMethods;
}

Method &Reflect::getMethod(const char *methodName) {
    return declaredMethods[0];
}

int Reflect::getClassSize() {
    return 0;
}

void* Reflect::getClassInstanceByName(string& name){
    auto i = classMap.find(name);
    if (i == classMap.end())
        return nullptr;
    return i->second();
}