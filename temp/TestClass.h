//
// Created by Alienson on 5.2.2024..
//

#ifndef WINTER_TESTCLASS_H
#define WINTER_TESTCLASS_H

#include "../source/reflect/Reflect.h"
#include "../source/http/ControllerMapping.h"

#include "memory"
#include "vector"

class TestClass : public Reflect {
public:
    TestClass() : reference(x) {
        int someRef = 5;
        reference = someRef;
        reference = x;
    }

    TestClass(int _x, std::vector<int> _vec, std::shared_ptr<int> _shared, int _reference, TestClass* _clazz) : reference(x) {
        x = _x;
        vec = _vec;
        shared = &_shared;
        reference = _reference;
        clazz = _clazz;
    }

    int getX(){
        return x;
    }

    std::vector<int>& getVec(){
        return vec;
    }

$GET("/home")
    static HttpResponse* handleData(HttpRequest* req);

private:
    int x;
    std::vector<int> vec;
    std::shared_ptr<int> *shared;
    int reference;
    TestClass* clazz;
};


class NoReflectClass{
public:
    NoReflectClass() {}
    int y;
    int z;
};


#endif //WINTER_TESTCLASS_H
