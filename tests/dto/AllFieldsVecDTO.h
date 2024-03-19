//
// Created by alienson on 17.3.24..
//

#ifndef WINTER_ALLFIELDSVECDTO_H
#define WINTER_ALLFIELDSVECDTO_H

#include "tests/dto/InnerClass.h"

class AllFieldsVecDTO : public Reflect {
public:
    vector<int> i;
    vector<float> f;
    vector<string> s;
    vector<InnerClass*> innerClassPtr;

    AllFieldsVecDTO() = default;

    static inline std::vector<Field> declaredFields = {};
    static inline std::vector<Method> declaredMethods = {};
    Field *getField(const char *fieldName) override {
        for (Field& f : declaredFields){
            if (f.name == fieldName)
                return &f;
        }
        return &Field::INVALID;
    }

    std::vector<Field> &getDeclaredFields() override {
        return declaredFields;
    }

    int getClassSize() override{
        return sizeof(AllFieldsDTO);
    }


    static void initializeReflection(){
        AllFieldsVecDTO* _allfieldsdto_ = (AllFieldsVecDTO*) malloc(sizeof(AllFieldsVecDTO));
        AllFieldsVecDTO::declaredFields.emplace_back("i","vector<int>",10,"AllFieldsVecDTO",(int*)(&_allfieldsdto_->i) - (int*)_allfieldsdto_);
        AllFieldsVecDTO::declaredFields.emplace_back("f","vector<float>",10,"AllFieldsVecDTO",(int*)(&_allfieldsdto_->f) - (int*)_allfieldsdto_);
        AllFieldsVecDTO::declaredFields.emplace_back("s","vector<string>",10,"AllFieldsVecDTO",(int*)(&_allfieldsdto_->s) - (int*)_allfieldsdto_);
        AllFieldsVecDTO::declaredFields.emplace_back("innerClassPtr","vector<InnerClass*>",10,"AllFieldsVecDTO",(int*)(&_allfieldsdto_->innerClassPtr) - (int*)_allfieldsdto_);
        free(_allfieldsdto_);
    }
};

#endif //WINTER_ALLFIELDSVECDTO_H
