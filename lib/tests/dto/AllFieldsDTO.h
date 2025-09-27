//
// Created by alienson on 17.3.24.
//

#ifndef WINTER_ALLFIELDSDTO_H
#define WINTER_ALLFIELDSDTO_H

#include "InnerClass.h"

class AllFieldsDTO : public Reflect {
public:
    short s;
    int i;
    long l;
    float f;
    double d;
    char c;
    std::string str;
    bool b;
    std::byte bt;

    short* sPtr;
    int* iPtr;
    long* lPtr;
    float* fPtr;
    double* dPtr;
    char* cPtr;
    std::string* strPtr;
    bool *bPtr;
    std::byte *btPtr;

    InnerClass* innerClassPtr;
    InnerClass innerClass;

    std::vector<int> vec;
    std::vector<int>* vecPtr;

    static inline std::vector<Field> declaredFields = {};
    static inline std::vector<Method> declaredMethods = {};
    Field *getField(const char *fieldName) const override {
        for (Field& f : declaredFields){
            if (f.name == fieldName)
                return &f;
        }
        return &Field::INVALID;
    }

    std::vector<Field> &getDeclaredFields() override {
        return declaredFields;
    }

    int getClassSize() const override{
        return sizeof(AllFieldsDTO);
    }

    static Reflect* getInstance(){
        return new AllFieldsDTO();
    }


    [[nodiscard]] Reflect* clone(CopyType copyType) const override{
        AllFieldsDTO* copy = new AllFieldsDTO();
        copy->s = this->s;
        copy->i = this->i;
        copy->l = this->l;
        copy->f = this->f;
        copy->d = this->d;
        copy->c = this->c;
        copy->str = this->str;
        copy->b = this->b;
        copy->bt = this->bt;
        copy->sPtr = this->sPtr;
        copy->iPtr = this->iPtr;
        copy->lPtr = this->lPtr;
        copy->fPtr = this->fPtr;
        copy->dPtr = this->dPtr;
        copy->cPtr = this->cPtr;
        copy->strPtr = this->strPtr;
        copy->bPtr = this->bPtr;
        copy->btPtr = this->btPtr;
        copy->innerClassPtr = this->innerClassPtr;
        copy->innerClass = this->innerClass;
        copy->vec = this->vec;
        copy->vecPtr = this->vecPtr;
        return copy;
    }

    static void initializeReflection(){
        AllFieldsDTO* _allfieldsdto_ = (AllFieldsDTO*) malloc(sizeof(AllFieldsDTO));
        AllFieldsDTO::declaredFields.emplace_back("s","short",0,"AllFieldsDTO",(int*)(&_allfieldsdto_->s) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("i","int",1,"AllFieldsDTO",(int*)(&_allfieldsdto_->i) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("l","long",2,"AllFieldsDTO",(int*)(&_allfieldsdto_->l) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("f","float",3,"AllFieldsDTO",(int*)(&_allfieldsdto_->f) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("d","double",4,"AllFieldsDTO",(int*)(&_allfieldsdto_->d) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("c","char",5,"AllFieldsDTO",(int*)(&_allfieldsdto_->c) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("str","string",8,"AllFieldsDTO",(int*)(&_allfieldsdto_->str) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("b","bool",6,"AllFieldsDTO",(int*)(&_allfieldsdto_->b) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("bt","byte",7,"AllFieldsDTO",(int*)(&_allfieldsdto_->bt) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("sPtr","short*",0,"AllFieldsDTO",(int*)(&_allfieldsdto_->sPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("iPtr","int*",1,"AllFieldsDTO",(int*)(&_allfieldsdto_->iPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("lPtr","long*",2,"AllFieldsDTO",(int*)(&_allfieldsdto_->lPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("fPtr","float*",3,"AllFieldsDTO",(int*)(&_allfieldsdto_->fPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("dPtr","double*",4,"AllFieldsDTO",(int*)(&_allfieldsdto_->dPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("cPtr","char*",5,"AllFieldsDTO",(int*)(&_allfieldsdto_->cPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("strPtr","string*",8,"AllFieldsDTO",(int*)(&_allfieldsdto_->strPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("bPtr","bool*",6,"AllFieldsDTO",(int*)(&_allfieldsdto_->bPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("btPtr","byte*",7,"AllFieldsDTO",(int*)(&_allfieldsdto_->btPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("innerClassPtr","InnerClass*",10,"AllFieldsDTO",(int*)(&_allfieldsdto_->innerClassPtr) - (int*)_allfieldsdto_,1,0);
        AllFieldsDTO::declaredFields.emplace_back("innerClass","InnerClass",10,"AllFieldsDTO",(int*)(&_allfieldsdto_->innerClass) - (int*)_allfieldsdto_,0,0);
        AllFieldsDTO::declaredFields.emplace_back("vec","vector<int>",9,"AllFieldsDTO",(int*)(&_allfieldsdto_->vec) - (int*)_allfieldsdto_,0,1);
        AllFieldsDTO::declaredFields.emplace_back("vecPtr","vector<int>*",9,"AllFieldsDTO",(int*)(&_allfieldsdto_->vecPtr) - (int*)_allfieldsdto_,1,1);
        free(_allfieldsdto_);
    }
};

#endif //WINTER_ALLFIELDSDTO_H
