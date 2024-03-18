//
// Created by Alienson on 8.3.2024..
//

#ifndef WINTER_INNERCLASS_H
#define WINTER_INNERCLASS_H

#include <utility>

#include "Reflect.h"

class InnerClass : public Reflect{
public:
    float x;
    double y;
    string c;

    InnerClass() : x(0), y(0), c() {}

    InnerClass(float x_, double y_, string c_) : x(x_), y(y_), c(std::move(c_)){}

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
        return sizeof(InnerClass);
	}


	static void initializeReflection(){
		InnerClass* _innerclass_ = (InnerClass*) malloc(sizeof(InnerClass));
		InnerClass::declaredFields.emplace_back("x","float",3,"InnerClass",(int*)(&_innerclass_->x) - (int*)_innerclass_);
		InnerClass::declaredFields.emplace_back("y","double",4,"InnerClass",(int*)(&_innerclass_->y) - (int*)_innerclass_);
		InnerClass::declaredFields.emplace_back("c","string",6,"InnerClass",(int*)(&_innerclass_->c) - (int*)_innerclass_);
		free(_innerclass_);
	}
};

#endif //WINTER_INNERCLASS_H
