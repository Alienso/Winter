//
// Created by Alienson on 14.2.2024.
//

#ifndef WINTER_JSONSERIALIZER_H
#define WINTER_JSONSERIALIZER_H

#include "reflect/Reflect.h"
#include "log/Logger.h"
#include "serialize/FieldTypeUtil.h"
#include "util/stringUtils.h"

class JsonSerializer {
public:
    [[nodiscard]] static std::string* serialize(Reflect* obj);

private:
    [[nodiscard]] static std::string* convertToJsonString(const Field &field, Reflect* obj);
    [[nodiscard]] static std::string* convertVectorToJsonString(const Field &field, Reflect *obj);


    [[nodiscard]] static std::string* vectorToString(const std::vector<char>& source);
    [[nodiscard]] static std::string* vectorToString(const std::vector<std::string>& source);
    [[nodiscard]] static std::string* vectorToString(const std::vector<Reflect*>& source);
    [[nodiscard]] static std::string* vectorToString(const std::vector<std::byte>& source);

    [[nodiscard]] static std::string* vectorPtrToString(const std::vector<char*>& source);
    [[nodiscard]] static std::string* vectorPtrToString(const std::vector<std::string*>& source);


    //TODO this has a copy in field but returning string instead of string*
    template<typename T>
    static std::string* serializeField(const Field& field, Reflect* obj, T (Field::*getFunc)(void*) const, std::string (*to_string_func)(T)){
        T t;
        if (!field.isPtr) {
            t = (field.*getFunc)(obj);
            return new std::string(to_string_func(t));
        }
        else{
            void* ptr = *field.getPtr(obj);
            if (ptr == nullptr)
                return new std::string("null");
            t = *(T*)ptr;
            return new std::string(to_string_func(t));
        }
    }

    template<typename U>
    [[nodiscard]] static std::string* vectorToString(const std::vector<U>& source, std::string (*parseFunc)(U val)){

        if (source.empty())
            return new std::string("[]");

        auto* res = new std::string("[");
        for(U u : source){
            std::string s = parseFunc(u);
            *res += s + ',';
        }
        (*res)[res->size()-1] = ']';
        return res;
    }

    template<typename U>
    [[nodiscard]] static std::string* vectorPtrToString(const std::vector<U*>& source, std::string (*parseFunc)(U val)){

        if (source.empty())
            return new std::string("[]");

        auto* res = new std::string("[");
        for(U* u : source){
            std::string s = parseFunc(*u);
            *res += s + ',';
        }
        (*res)[res->size()-1] = ']';
        return res;
    }

    template<typename T>
    static std::string* handleVecData(const Field& f, Reflect* obj, const bool isElemPtr, std::string (*toStr)(T)){
        if (!f.isPtr) {
            if (!isElemPtr) {
                auto* vec = static_cast<std::vector<T> *>(f.getAddress(obj));
                return vectorToString(*vec, toStr);
            }else{
                auto* vecPtr = static_cast<std::vector<T*> *>(f.getAddress(obj));
                return vectorPtrToString(*vecPtr, toStr);
            }
        }else {
            if (!isElemPtr) {
                auto* vec = static_cast<std::vector<T> *>(*(f.getPtr(obj)));
                return vectorToString(*vec, toStr);
            }else{
                auto* vecPtr = static_cast<std::vector<T*> *>(*(f.getPtr(obj)));
                return vectorPtrToString(*vecPtr, toStr);
            }
        }
    }

};


#endif //WINTER_JSONSERIALIZER_H
