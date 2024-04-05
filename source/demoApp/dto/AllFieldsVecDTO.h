//
// Created by alienson on 17.3.24..
//

#ifndef WINTER_ALLFIELDSVECDTO_H
#define WINTER_ALLFIELDSVECDTO_H

#include "tests/dto/InnerClass.h"

class AllFieldsVecDTO : public Reflect {
public:
    vector<short> s;
    vector<int> i;
    vector<long> l;
    vector<float> f;
    vector<double> d;
    vector<char> c;
    vector<string> str;
    vector<bool> b;
    vector<byte> bt;

    vector<short*> sPtr;
    vector<int*> iPtr;
    vector<long*> lPtr;
    vector<float*> fPtr;
    vector<double*> dPtr;
    vector<char*> cPtr;
    vector<string*> strPtr;
    vector<bool*> bPtr;
    vector<byte*> btPtr;

    vector<InnerClass*> innerClassPtr;
    vector<InnerClass> innerClass;

    vector<int>* vecPtrInt;
    vector<int*>* vecPtrIntPtr;

    vector<string>* vecPtrString;
    vector<string*>* vecPtrStringPtr;

    vector<InnerClass>* vecPtrInnerClass;
    vector<InnerClass*>* vecPtrInnerClassPtr;

    AllFieldsVecDTO() = default;

};

#endif //WINTER_ALLFIELDSVECDTO_H
