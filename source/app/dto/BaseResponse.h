//
// Created by Alienson on 5.3.2024..
//

#ifndef WINTER_BASERESPONSE_H
#define WINTER_BASERESPONSE_H

#include "../../reflect/Reflect.h"

class BaseResponse : public Reflect{
public:
    int code;
    string message;
};

#endif //WINTER_BASERESPONSE_H