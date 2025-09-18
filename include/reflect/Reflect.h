//
// Created by Alienson on 6.2.2024.
//

/**
 * Reflect.cpp is further modified by preprocessor in order to fill classMap and generate function getClassInstanceByName
 */

#ifndef WINTER_REFLECT_H
#define WINTER_REFLECT_H

#include "reflect/Field.h"
#include "reflect/Method.h"

#include <vector>
#include <unordered_map>

class Reflect {
public:

    Reflect() = default;
    virtual ~Reflect()= default;

    [[nodiscard]] virtual std::vector<Field>& getDeclaredFields();
    [[nodiscard]] virtual Field* getField(const char* fieldName) const;

    [[nodiscard]] virtual std::vector<Method>& getDeclaredMethods();
    [[nodiscard]] virtual Method& getMethod(const char* methodName);

    [[nodiscard]] virtual int getClassSize() const;
    [[nodiscard]] static Reflect* getClassInstanceByName(const std::string& name);

    [[nodiscard]] static Reflect* getInstance();
    [[nodiscard]] virtual Reflect* clone(CopyType copyType) const;

    static void initializeReflection();
    static void initializeClassMap();

    static inline std::vector<Field> declaredFields = {};
    static inline std::vector<Method> declaredMethods = {};

    static inline std::unordered_map<std::string, Reflect* (*)()> classMap;
};


#endif //WINTER_REFLECT_H
