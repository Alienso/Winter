//
// Created by Alienson on 6.2.2024..
//

#ifndef WINTER_REFLECT_H
#define WINTER_REFLECT_H

#include "Field.h"
#include "Method.h"

#include <vector>
#include <unordered_map>

class Reflect {
public:
    virtual std::vector<Field>& getDeclaredFields();
    virtual Field* getField(const char* fieldName);

    virtual std::vector<Method>& getDeclaredMethods();
    virtual Method& getMethod(const char* methodName);

    virtual int getClassSize();

    static void* getClassInstanceByName(string& name);

    static inline std::vector<Field> declaredFields = {};
    static inline std::vector<Method> declaredMethods = {};

    static inline std::unordered_map<string,Reflect* (*)()> classMap;

    static void initializeReflection();
    static void initializeClassMap();
};


#endif //WINTER_REFLECT_H
