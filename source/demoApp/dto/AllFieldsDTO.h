//
// Created by alienson on 17.3.24..
//

#ifndef WINTER_ALLFIELDSDTO_H
#define WINTER_ALLFIELDSDTO_H

#include "tests/dto/InnerClass.h"

class AllFieldsDTO : public Reflect {
public:
    short s;
    int i;
    long l;
    float f;
    double d;
    char c;
    string str;
    bool b;
    std::byte bt;

    short* sPtr;
    int* iPtr;
    long* lPtr;
    float* fPtr;
    double* dPtr;
    char* cPtr;
    string* strPtr;
    bool *bPtr;
    std::byte *btPtr;

    InnerClass* innerClassPtr;
    InnerClass innerClass;

    std::vector<int> vec;
    std::vector<int>* vecPtr;

    AllFieldsDTO(){};
};

#endif //WINTER_ALLFIELDSDTO_H
