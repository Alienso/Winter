//
// Created by Alienson on 8.3.2024.
//

#ifndef WINTER_INNERCLASS_H
#define WINTER_INNERCLASS_H

#include <utility>

#include "reflect/Reflect.h"

class InnerClass : public Reflect{
public:
    float x;
    double y;
    std::string c;

    InnerClass(){}
    InnerClass(float x, double y, std::string c) : x(x), y(y), c(std::move(c)){}

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
        return sizeof(InnerClass);
    }

    static Reflect* getInstance(){
        return new InnerClass();
    }


    [[nodiscard]] Reflect* clone(CopyType copyType) const override{
        InnerClass* copy = new InnerClass();
        copy->x = this->x;
        copy->y = this->y;
        copy->c = this->c;
        return copy;
    }

    static void initializeReflection(){
        InnerClass* _innerclass_ = (InnerClass*) malloc(sizeof(InnerClass));
        InnerClass::declaredFields.emplace_back("x","float",3,"InnerClass",(int*)(&_innerclass_->x) - (int*)_innerclass_,0,0);
        InnerClass::declaredFields.emplace_back("y","double",4,"InnerClass",(int*)(&_innerclass_->y) - (int*)_innerclass_,0,0);
        InnerClass::declaredFields.emplace_back("c","string",8,"InnerClass",(int*)(&_innerclass_->c) - (int*)_innerclass_,0,0);
        free(_innerclass_);
    }
};

#endif //WINTER_INNERCLASS_H