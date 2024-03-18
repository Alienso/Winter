//
// Created by alienson on 17.3.24..
//

#ifndef WINTER_ALLFIELDSDTO_H
#define WINTER_ALLFIELDSDTO_H

#include "tests/dto/InnerClass.h"

class AllFieldsDTO : public Reflect {
public:
    int i;
    float f;
    string s;
    vector<int> vec;
    InnerClass* innerClassPtr;
    InnerClass innerClassObj;

    AllFieldsDTO() = default;

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
		auto* _allfieldsdto_ = (AllFieldsDTO*) malloc(sizeof(AllFieldsDTO));
		AllFieldsDTO::declaredFields.emplace_back("i","int",1,"AllFieldsDTO",(int*)(&_allfieldsdto_->i) - (int*)_allfieldsdto_);
		AllFieldsDTO::declaredFields.emplace_back("f","float",3,"AllFieldsDTO",(int*)(&_allfieldsdto_->f) - (int*)_allfieldsdto_);
		AllFieldsDTO::declaredFields.emplace_back("s","string",6,"AllFieldsDTO",(int*)(&_allfieldsdto_->s) - (int*)_allfieldsdto_);
		AllFieldsDTO::declaredFields.emplace_back("vec","vector<int>",10,"AllFieldsDTO",(int*)(&_allfieldsdto_->vec) - (int*)_allfieldsdto_);
		AllFieldsDTO::declaredFields.emplace_back("innerClassPtr","InnerClass*",8,"AllFieldsDTO",(int*)(&_allfieldsdto_->innerClassPtr) - (int*)_allfieldsdto_);
        AllFieldsDTO::declaredFields.emplace_back("innerClassObj","InnerClass",7,"AllFieldsDTO",(int*)(&_allfieldsdto_->innerClassObj) - (int*)_allfieldsdto_);
        free(_allfieldsdto_);
	}
};

#endif //WINTER_ALLFIELDSDTO_H
