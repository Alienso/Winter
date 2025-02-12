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
};

#endif //WINTER_BASEREQUEST_H

