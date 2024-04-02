//
// Created by alienson on 1.4.24..
//

#ifndef WINTER_DTOUTILS_H
#define WINTER_DTOUTILS_H

#include "../dto/AllFieldsDTO.h"
#include "../dto/AllFieldsVecDTO.h"
#include "../dto/BaseRequest.h"

template<typename T>
vector<T*> createVecPtr(vector<T> vec){
    vector<T*> res = {};
    for (T t : vec){
        T* x = new T(t);
        res.push_back(x);
    }

    return res;
}

inline void fillFieldData(AllFieldsDTO& dto){
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

inline void fillPtrData(AllFieldsDTO& dto){

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

inline void fillVectorFieldData(AllFieldsVecDTO& dto){
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

inline void fillVectorPtrData(AllFieldsVecDTO& dto){
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

#endif //WINTER_DTOUTILS_H