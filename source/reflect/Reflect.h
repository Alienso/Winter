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

    virtual ~Reflect()= default;

    [[nodiscard]] virtual std::vector<Field>& getDeclaredFields();
    [[nodiscard]] virtual Field* getField(const char* fieldName);

    [[nodiscard]] virtual std::vector<Method>& getDeclaredMethods();
    [[nodiscard]] virtual Method& getMethod(const char* methodName);

    [[nodiscard]] virtual int getClassSize();
    [[nodiscard]] static void* getClassInstanceByName(string& name);

    static void initializeReflection();
    static void initializeClassMap();

    static inline std::vector<Field> declaredFields = {};
    static inline std::vector<Method> declaredMethods = {};

    static inline std::unordered_map<string,Reflect* (*)()> classMap;
};


#endif //WINTER_REFLECT_H
