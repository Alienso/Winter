//
// Created by Alienson on 6.2.2024..
//

#include "Reflect.h"

#include "Logger.h"

std::vector<Field> &Reflect::getDeclaredFields() {
    wtLogError("getDeclaredFields called for Reflect*!");
    return declaredFields;
}

Field *Reflect::getField(const char *fieldName) {
    wtLogError("getField called for %s for Reflect*!", fieldName);
    return &Field::INVALID;
}

std::vector<Method> &Reflect::getDeclaredMethods() {
    wtLogError("getDeclaredMethods called for Reflect*!");
    return declaredMethods;
}

Method &Reflect::getMethod(const char *methodName) {
    wtLogError("getMethod %s called for Reflect*!", methodName);
    return declaredMethods[0];
}

int Reflect::getClassSize() {
    wtLogError("getClassSize called for Reflect*!");
    return 0;
}

void* Reflect::getClassInstanceByName(const string& name){
    auto i = classMap.find(name);
    if (i == classMap.end())
        return nullptr;
    return i->second();
}

Reflect* Reflect::getInstance(){
    wtLogError("getInstance called for Reflect*");
    return nullptr;
}