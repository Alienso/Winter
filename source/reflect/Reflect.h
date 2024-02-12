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
    static std::vector<Field>& getDeclaredFields();
    static Field* getField(const char* fieldName);

    static std::vector<Method>& getDeclaredMethods();
    static Method& getMethod(const char* methodName);

    static std::vector<Field> declaredFields;
    static std::vector<Method> declaredMethods;
};


#endif //WINTER_REFLECT_H
