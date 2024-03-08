//
// Created by Alienson on 6.2.2024..
//

#include "Reflect.h"

#include "../log/Logger.h"

#include "../app/dto/BaseRequest.h"
#include "../app/dto/BaseResponse.h"

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
    return _reflect_class_size_;
}

void* Reflect::getClassInstanceByName(string& name){
    auto i = classMap.find(name);
    if (i == classMap.end())
        return nullptr;
    return i->second();
}

//To be generated

void Reflect::initializeReflection() {
    BaseRequest::initializeReflection();
    BaseResponse::initializeReflection();
    InnerClass::initializeReflection();

    initializeClassMap();
}


Reflect* baseRequest(){
    return new BaseRequest();
}

Reflect* innerClass(){
    return new InnerClass();
}

void Reflect::initializeClassMap(){
    Reflect::classMap["BaseRequest"] = &baseRequest;
    Reflect::classMap["InnerClass"] = &innerClass;
}