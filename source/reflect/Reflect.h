//
// Created by Alienson on 6.2.2024..
//

#ifndef WINTER_REFLECT_H
#define WINTER_REFLECT_H

#include <vector>
#include "Field.h"
#include "Method.h"

class Reflect {
public:
    virtual std::vector<Field>& getDeclaredFields();
    virtual Field* getField(const char* fieldName);

    virtual std::vector<Method>& getDeclaredMethods();
    virtual Method& getMethod(const char* methodName);

    static inline std::vector<Field> declaredFields = {};
    static inline std::vector<Method> declaredMethods = {};
};


#endif //WINTER_REFLECT_H
