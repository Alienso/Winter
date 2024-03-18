//
// Created by Alienson on 5.3.2024..
//

#include "Reflect.h"
#include "InnerClass.h"

#ifndef WINTER_BASEREQUEST_H
#define WINTER_BASEREQUEST_H

class BaseRequest : public Reflect{
public:
    int number;
    string type;
    InnerClass* innerClass;
    vector<int> values;

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
        return sizeof(BaseRequest);
	}


	static void initializeReflection(){
		BaseRequest* _baserequest_ = (BaseRequest*) malloc(sizeof(BaseRequest));
		BaseRequest::declaredFields.emplace_back("number","int",1,"BaseRequest",(int*)(&_baserequest_->number) - (int*)_baserequest_);
		BaseRequest::declaredFields.emplace_back("type","string",6,"BaseRequest",(int*)(&_baserequest_->type) - (int*)_baserequest_);
		BaseRequest::declaredFields.emplace_back("innerClass","InnerClass*",8,"BaseRequest",(int*)(&_baserequest_->innerClass) - (int*)_baserequest_);
		BaseRequest::declaredFields.emplace_back("values","vector<int>",10,"BaseRequest",(int*)(&_baserequest_->values) - (int*)_baserequest_);
		free(_baserequest_);
	}
};

#endif //WINTER_BASEREQUEST_H

