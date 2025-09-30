//
// Created by Alienson on 27.1.2024.
//

#define CATCH_CONFIG_MAIN

#include "reflect/Reflect.h"
#include "./dto/AllFieldsDTO.h"
#include "./dto/AllFieldsVecDTO.h"
#include "./dto/InnerClass.h"
#include "../include/core/Component.h"

Reflect* _AllFieldsDTO_(){
    return new AllFieldsDTO();
}

Reflect* _AllFieldsVecDTO_(){
    return new AllFieldsVecDTO();
}

Reflect* _InnerClass_(){
    return new InnerClass();
}

void Component::initializeComponents() {

}

void Reflect::initializeClassMap(){
    Reflect::classMap["AllFieldsDTO"] = &_AllFieldsDTO_;
    Reflect::classMap["AllFieldsVecDTO"] = &_AllFieldsVecDTO_;
    Reflect::classMap["InnerClass"] = &_InnerClass_;
}
void Reflect::initializeReflection() {
    AllFieldsDTO::initializeReflection();
    AllFieldsVecDTO::initializeReflection();
    InnerClass::initializeReflection();

    initializeClassMap();
}

static int __init__ = ([]{
    AllFieldsDTO::initializeReflection();
    AllFieldsVecDTO::initializeReflection();
    InnerClass::initializeReflection();

    Reflect::classMap["AllFieldsDTO"] = &_AllFieldsDTO_;
    Reflect::classMap["AllFieldsVecDTO"] = &_AllFieldsVecDTO_;
    Reflect::classMap["InnerClass"] = &_InnerClass_;

    return 0;
})();

//#include "loggerTests.h"
#include "httpRequestParseTests.h"
#include "deserializationTests.h"
#include "serializationTests.h"
#include "stringUtilsTests.h"
#include "reflectTests.h"
