//
// Created by Alienson on 5.3.2024.
//

#include "reflect/Reflect.h"
#include "InnerClass.h"

#ifndef WINTER_BASEREQUEST_H
#define WINTER_BASEREQUEST_H

class BaseRequest : public Reflect{
public:
    int number;
    std::string type;
    InnerClass* innerClass;
    std::vector<int> values;

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
        return sizeof(BaseRequest);
    }

    static Reflect* getInstance(){
        return new BaseRequest();
    }


    [[nodiscard]] Reflect* clone(CopyType copyType) const override{
        BaseRequest* copy = new BaseRequest();
        copy->number = this->number;
        copy->type = this->type;
        copy->innerClass = this->innerClass;
        copy->values = this->values;
        return copy;
    }

    static void initializeReflection(){
        BaseRequest* _baserequest_ = (BaseRequest*) malloc(sizeof(BaseRequest));
        BaseRequest::declaredFields.emplace_back("number","int",1,"BaseRequest",(int*)(&_baserequest_->number) - (int*)_baserequest_,0,0);
        BaseRequest::declaredFields.emplace_back("type","string",8,"BaseRequest",(int*)(&_baserequest_->type) - (int*)_baserequest_,0,0);
        BaseRequest::declaredFields.emplace_back("innerClass","InnerClass*",10,"BaseRequest",(int*)(&_baserequest_->innerClass) - (int*)_baserequest_,1,0);
        BaseRequest::declaredFields.emplace_back("values","vector<int>",9,"BaseRequest",(int*)(&_baserequest_->values) - (int*)_baserequest_,0,1);
        free(_baserequest_);
    }
};

#endif //WINTER_BASEREQUEST_H

