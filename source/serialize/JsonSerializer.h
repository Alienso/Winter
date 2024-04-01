//
// Created by Alienson on 14.2.2024..
//

#ifndef WINTER_JSONSERIALIZER_H
#define WINTER_JSONSERIALIZER_H

#include "Reflect.h"
#include "Logger.h"
#include "FieldTypeUtil.h"
#include "stringUtils.h"

using namespace std;

class JsonSerializer {
public:
    [[nodiscard]] static string* serialize(Reflect* obj);

private:
    [[nodiscard]] static string* convertToJsonString(const Field &field, Reflect* obj);
    [[nodiscard]] static string* convertVectorToJsonString(const Field &field, Reflect *obj);


    [[nodiscard]] static string* vectorToString(const vector<char>& source);
    [[nodiscard]] static string* vectorToString(const vector<string>& source);
    [[nodiscard]] static string* vectorToString(const vector<Reflect*>& source);
    [[nodiscard]] static string* vectorToString(const vector<byte>& source);

    [[nodiscard]] static string* vectorPtrToString(const vector<char*>& source);
    [[nodiscard]] static string* vectorPtrToString(const vector<string*>& source);


    template<typename T>
    static string* serializeField(const Field& field, Reflect* obj, T (Field::*getFunc)(void*) const, string (*to_string_func)(T)){
        T t;
        if (!field.isPtr) {
            t = (field.*getFunc)(obj);
            return new string(to_string_func(t));
        }
        else{
            void* ptr = *field.getPtr(obj);
            if (ptr == nullptr)
                return new string("null");
            t = *(T*)ptr;
            return new string(to_string_func(t));
        }
    }

    template<typename U>
    [[nodiscard]] static string* vectorToString(const vector<U>& source, string (*parseFunc)(U val)){

        if (source.empty())
            return new string("[]");

        auto* res = new string("[");
        for(U u : source){
            string s = parseFunc(u);
            *res += s + ',';
        }
        (*res)[res->size()-1] = ']';
        return res;
    }

    template<typename U>
    [[nodiscard]] static string* vectorPtrToString(const vector<U*>& source, string (*parseFunc)(U val)){

        if (source.empty())
            return new string("[]");

        auto* res = new string("[");
        for(U* u : source){
            string s = parseFunc(*u);
            *res += s + ',';
        }
        (*res)[res->size()-1] = ']';
        return res;
    }

    template<typename T>
    static string* handleVecData(const Field& f, Reflect* obj, const bool isElemPtr, string (*toStr)(T)){
        if (!f.isPtr) {
            if (!isElemPtr) {
                auto* vec = static_cast<vector<T> *>(f.getAddress(obj));
                return vectorToString(*vec, toStr);
            }else{
                auto* vecPtr = static_cast<vector<T*> *>(f.getAddress(obj));
                return vectorPtrToString(*vecPtr, toStr);
            }
        }else {
            if (!isElemPtr) {
                auto* vec = static_cast<vector<T> *>(*(f.getPtr(obj)));
                return vectorToString(*vec, toStr);
            }else{
                auto* vecPtr = static_cast<vector<T*> *>(*(f.getPtr(obj)));
                return vectorPtrToString(*vecPtr, toStr);
            }
        }
    }

};


#endif //WINTER_JSONSERIALIZER_H
