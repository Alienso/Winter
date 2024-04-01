//
// Created by alienson on 18.3.24..
//

#ifndef WINTER_SERIALIZATIONTESTS_H
#define WINTER_SERIALIZATIONTESTS_H

#include "Reflect.h"

#include "JsonSerializer.h"
#include "./dto/AllFieldsDTO.h"
#include "./dto/BaseRequest.h"

template<typename T>
vector<T*> createVecPtr(vector<T> vec){
    vector<T*> res = {};
    for (T t : vec){
        T* x = new T(t);
        res.push_back(x);
    }

    return res;
}

void fillFieldData(AllFieldsDTO& dto){
    dto.s = 1;
    dto.i = 5;
    dto.l = 10;
    dto.f = 5.5;
    dto.d = 1.25;
    dto.c = 'a';
    dto.b = false;
    dto.str = "Hello World";
    dto.vec = {1,2,3};
    InnerClass clazz{5,2.5,"c"};
    dto.innerClass = clazz;
}

void fillPtrData(AllFieldsDTO& dto){

    dto.sPtr = new short(1);
    dto.iPtr = new int(5);
    dto.lPtr = new long(10);
    dto.fPtr = new float(5.5);
    dto.dPtr = new double(1.25);
    dto.cPtr = new char('a');
    dto.bPtr = new bool(false);
    dto.strPtr = new string("Hello World");
    dto.vecPtr = new vector<int>{1,2,3};
    auto* clazz = new InnerClass{5,2.5,"c"};
    dto.innerClassPtr = clazz;

    dto.btPtr = nullptr;
}

void fillVectorFieldData(AllFieldsVecDTO& dto){
    dto.s = {1,2,3};
    dto.i = {5,4,3};
    dto.l = {10,15,20};
    dto.f = {5.5,2};
    dto.d = {1.25,0};
    dto.c = {'a','b'};
    dto.b = {false, true};
    dto.str = {"Hello", "World"};
    dto.bt = {};

    InnerClass clazz{5,2.5,"c"};
    InnerClass clazz2{1,0,"s"};
    vector<InnerClass> vecClass{};
    vecClass.push_back(clazz);
    vecClass.push_back(clazz2);
    dto.innerClass = vecClass;
}

void fillVectorPtrData(AllFieldsVecDTO& dto){
    dto.sPtr = createVecPtr<short>({1,2,3});
    dto.iPtr = createVecPtr<int>({5,4,3});
    dto.lPtr = createVecPtr<long>({10,15,20});
    dto.fPtr = createVecPtr<float>({5.5,2});
    dto.dPtr = createVecPtr<double>({1.25,0});
    dto.cPtr = createVecPtr<char>({'a','b'});
    dto.bPtr = createVecPtr<bool>({false, true});
    dto.strPtr = createVecPtr<string>({"Hello", "World"});
    dto.btPtr = {};

    InnerClass* clazz = new InnerClass{5,2.5,"c"};
    InnerClass* clazz2 = new InnerClass{1,0,"s"};
    dto.innerClassPtr = {clazz, clazz2};

    dto.vecPtrInt = new vector<int>({1,2,3});
    vector<int*>* vecIntPtrPtr = new vector<int*>(createVecPtr<int>({0,0,7}));
    dto.vecPtrIntPtr = vecIntPtrPtr;

    dto.vecPtrString = new vector<string>({"Hi", "World"});
    vector<string*>* vecStringPtrPtr = new vector<string*>(createVecPtr<string>({"World", "Hi"}));
    dto.vecPtrStringPtr = vecStringPtrPtr;

    dto.vecPtrInnerClass = new vector<InnerClass>({*clazz, *clazz2});
    vector<InnerClass*>* vecClassPtrPtr = new vector<InnerClass*>({clazz, clazz2});
    dto.vecPtrInnerClassPtr = vecClassPtrPtr;
}

TEST_CASE("Serialization Tests", "[JsonSerializer::serialize]"){

    AllFieldsDTO dto;
    fillFieldData(dto);
    fillPtrData(dto);
    InnerClass clazz{5,2.5,"c"};

    std::cout << "FIELD COUNT: " << dto.getDeclaredFields().size() << '\n';


    SECTION("Serializing InnerClass"){
        string expected = "{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n}";
        string* s = JsonSerializer::serialize(&clazz);
        REQUIRE(*s == expected);
    }

    SECTION("Serializing AllFieldsDTO"){
        string expected = "{\n\"s\":1,\n\"i\":5,\n\"l\":10,\n\"f\":5.500000,\n\"d\":1.250000,\n\"c\":\"97\",\n\"str\":\"Hello World\",\n\"b\":false,\n\"bt\":\"\",\n\"sPtr\":1,\n\"iPtr\":5,\n\"lPtr\":10,\n\"fPtr\":5.500000,\n\"dPtr\":1.250000,\n\"cPtr\":\"97\",\n\"strPtr\":\"Hello World\",\n\"bPtr\":false,\n\"btPtr\":\"\",\n\"innerClassPtr\":{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},\n\"innerClass\":{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},\n\"vec\":[1,2,3],\n\"vecPtr\":[1,2,3]\n}";
        string* s = JsonSerializer::serialize(&dto);
        REQUIRE(*s == expected);
    }

}

TEST_CASE("Vector Serialization Tests", "[JsonSerializer::serialize]"){

    auto* allFieldsVecDto = new AllFieldsVecDTO();
    fillVectorFieldData(*allFieldsVecDto);
    fillVectorPtrData(*allFieldsVecDto);

    /*SECTION("Serializing empty vec-s") {
        string expected = "{\n\"i\":[],\n\"f\":[],\n\"s\":[],\n\"innerClassPtr\":[]\n}";
        string *s = JsonSerializer::serialize(allFieldsVecDto);
        REQUIRE(*s == expected);
    }*/

    SECTION("Serializing vec<Reflect*>") {
        string expected = "{\n\"s\":[1,2,3],\n\"i\":[5,4,3],\n\"l\":[10,15,20],\n\"f\":[5.500000,2.000000],\n\"d\":[1.250000,0.000000],\n\"c\":[\"a\",\"b\"],\n\"str\":[\"Hello\",\"World\"],\n\"b\":[false,true],\n\"bt\":\"\",\n\"sPtr\":[1,2,3],\n\"iPtr\":[5,4,3],\n\"lPtr\":[10,15,20],\n\"fPtr\":[5.500000,2.000000],\n\"dPtr\":[1.250000,0.000000],\n\"cPtr\":[\"a\",\"b\"],\n\"strPtr\":[\"Hello\",\"World\"],\n\"bPtr\":[false,true],\n\"btPtr\":\"\",\n\"innerClassPtr\":[{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},{\n\"x\":1.000000,\n\"y\":0.000000,\n\"c\":\"s\"\n}],\n\"innerClass\":[{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},{\n\"x\":1.000000,\n\"y\":0.000000,\n\"c\":\"s\"\n}],\n\"vecPtrInt\":[1,2,3],\n\"vecPtrIntPtr\":[0,0,7],\n\"vecPtrString\":[\"Hi\",\"World\"],\n\"vecPtrStringPtr\":[\"World\",\"Hi\"],\n\"vecPtrInnerClass\":[{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},{\n\"x\":1.000000,\n\"y\":0.000000,\n\"c\":\"s\"\n}],\n\"vecPtrInnerClassPtr\":[{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},{\n\"x\":1.000000,\n\"y\":0.000000,\n\"c\":\"s\"\n}]\n}";
        string *s = JsonSerializer::serialize(allFieldsVecDto);
        REQUIRE(*s == expected);
    }
}

#endif //WINTER_SERIALIZATIONTESTS_H