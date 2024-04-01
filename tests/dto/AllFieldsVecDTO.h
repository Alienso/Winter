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

    int getClassSize() const override{
        return sizeof(AllFieldsVecDTO);
	}

	static Reflect* getInstance(){
    	return new AllFieldsVecDTO();
	}


	[[nodiscard]] Reflect* clone(CopyType copyType) const override{
		AllFieldsVecDTO* copy = new AllFieldsVecDTO();
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
		copy->vecPtrInt = this->vecPtrInt;
		copy->vecPtrIntPtr = this->vecPtrIntPtr;
		copy->vecPtrString = this->vecPtrString;
		copy->vecPtrStringPtr = this->vecPtrStringPtr;
		copy->vecPtrInnerClass = this->vecPtrInnerClass;
		copy->vecPtrInnerClassPtr = this->vecPtrInnerClassPtr;
	return copy;
	}

	static void initializeReflection(){
		AllFieldsVecDTO* _allfieldsvecdto_ = (AllFieldsVecDTO*) malloc(sizeof(AllFieldsVecDTO));
		AllFieldsVecDTO::declaredFields.emplace_back("s","vector<short>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->s) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("i","vector<int>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->i) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("l","vector<long>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->l) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("f","vector<float>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->f) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("d","vector<double>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->d) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("c","vector<char>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->c) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("str","vector<string>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->str) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("b","vector<bool>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->b) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("bt","vector<byte>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->bt) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("sPtr","vector<short*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->sPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("iPtr","vector<int*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->iPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("lPtr","vector<long*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->lPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("fPtr","vector<float*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->fPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("dPtr","vector<double*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->dPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("cPtr","vector<char*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->cPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("strPtr","vector<string*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->strPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("bPtr","vector<bool*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->bPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("btPtr","vector<byte*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->btPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("innerClassPtr","vector<InnerClass*>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->innerClassPtr) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("innerClass","vector<InnerClass>",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->innerClass) - (int*)_allfieldsvecdto_,0,1);
		AllFieldsVecDTO::declaredFields.emplace_back("vecPtrInt","vector<int>*",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->vecPtrInt) - (int*)_allfieldsvecdto_,1,1);
		AllFieldsVecDTO::declaredFields.emplace_back("vecPtrIntPtr","vector<int*>*",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->vecPtrIntPtr) - (int*)_allfieldsvecdto_,1,1);
		AllFieldsVecDTO::declaredFields.emplace_back("vecPtrString","vector<string>*",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->vecPtrString) - (int*)_allfieldsvecdto_,1,1);
		AllFieldsVecDTO::declaredFields.emplace_back("vecPtrStringPtr","vector<string*>*",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->vecPtrStringPtr) - (int*)_allfieldsvecdto_,1,1);
		AllFieldsVecDTO::declaredFields.emplace_back("vecPtrInnerClass","vector<InnerClass>*",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->vecPtrInnerClass) - (int*)_allfieldsvecdto_,1,1);
		AllFieldsVecDTO::declaredFields.emplace_back("vecPtrInnerClassPtr","vector<InnerClass*>*",9,"AllFieldsVecDTO",(int*)(&_allfieldsvecdto_->vecPtrInnerClassPtr) - (int*)_allfieldsvecdto_,1,1);
		free(_allfieldsvecdto_);
	}
};

#endif //WINTER_ALLFIELDSVECDTO_H
