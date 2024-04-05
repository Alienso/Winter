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

    InnerClass(){}
    InnerClass(float x, double y, string c) : x(x), y(y), c(std::move(c)){}
};

#endif //WINTER_INNERCLASS_H