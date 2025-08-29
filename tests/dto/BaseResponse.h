//
// Created by Alienson on 5.3.2024.
//

#ifndef WINTER_BASERESPONSE_H
#define WINTER_BASERESPONSE_H

#include "Reflect.h"
#include "InnerClass.h"

class BaseResponse : public Reflect{
public:
    int code;
    std::string message;
    InnerClass* innerClass;
    int sum;
};

#endif //WINTER_BASERESPONSE_H