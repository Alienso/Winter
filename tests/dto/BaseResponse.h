//
// Created by Alienson on 5.3.2024..
//

#ifndef WINTER_BASERESPONSE_H
#define WINTER_BASERESPONSE_H

#include "Reflect.h"
#include "InnerClass.h"

class BaseResponse : public Reflect{
public:
    int code;
    string message;
    InnerClass* innerClass;
    int sum;

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
        return sizeof(BaseResponse);
	}

	static Reflect* getInstance(){
    	return new BaseResponse();
	}


	[[nodiscard]] Reflect* clone(CopyType copyType) const override{
		BaseResponse* copy = new BaseResponse();
		copy->code = this->code;
		copy->message = this->message;
		copy->innerClass = this->innerClass;
		copy->sum = this->sum;
	return copy;
	}

	static void initializeReflection(){
		BaseResponse* _baseresponse_ = (BaseResponse*) malloc(sizeof(BaseResponse));
		BaseResponse::declaredFields.emplace_back("code","int",1,"BaseResponse",(int*)(&_baseresponse_->code) - (int*)_baseresponse_,0,0);
		BaseResponse::declaredFields.emplace_back("message","string",8,"BaseResponse",(int*)(&_baseresponse_->message) - (int*)_baseresponse_,0,0);
		BaseResponse::declaredFields.emplace_back("innerClass","InnerClass*",10,"BaseResponse",(int*)(&_baseresponse_->innerClass) - (int*)_baseresponse_,1,0);
		BaseResponse::declaredFields.emplace_back("sum","int",1,"BaseResponse",(int*)(&_baseresponse_->sum) - (int*)_baseresponse_,0,0);
		free(_baseresponse_);
	}
};

#endif //WINTER_BASERESPONSE_H
