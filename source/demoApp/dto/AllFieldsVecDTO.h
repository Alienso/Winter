//
// Created by alienson on 17.3.24.
//

#ifndef WINTER_ALLFIELDSVECDTO_H
#define WINTER_ALLFIELDSVECDTO_H

#include "InnerClass.h"

class AllFieldsVecDTO : public Reflect {
public:
    std::vector<short> s;
    std::vector<int> i;
    std::vector<long> l;
    std::vector<float> f;
    std::vector<double> d;
    std::vector<char> c;
    std::vector<std::string> str;
    std::vector<bool> b;
    std::vector<std::byte> bt;

    std::vector<short*> sPtr;
    std::vector<int*> iPtr;
    std::vector<long*> lPtr;
    std::vector<float*> fPtr;
    std::vector<double*> dPtr;
    std::vector<char*> cPtr;
    std::vector<std::string*> strPtr;
    std::vector<bool*> bPtr;
    std::vector<std::byte*> btPtr;

    std::vector<InnerClass*> innerClassPtr;
    std::vector<InnerClass> innerClass;

    std::vector<int>* vecPtrInt;
    std::vector<int*>* vecPtrIntPtr;

    std::vector<std::string>* vecPtrString;
    std::vector<std::string*>* vecPtrStringPtr;

    std::vector<InnerClass>* vecPtrInnerClass;
    std::vector<InnerClass*>* vecPtrInnerClassPtr;

    AllFieldsVecDTO() = default;

};

#endif //WINTER_ALLFIELDSVECDTO_H
