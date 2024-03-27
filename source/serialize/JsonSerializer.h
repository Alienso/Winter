//
// Created by Alienson on 14.2.2024..
//

#ifndef WINTER_JSONSERIALIZER_H
#define WINTER_JSONSERIALIZER_H

#include "Reflect.h"
#include "Logger.h"
#include "FieldTypeEnums.h"
#include "stringUtils.h"

using namespace std;

class JsonSerializer {
public:
    string* serialize(Reflect* obj);

private:
    [[nodiscard]] string convertToJsonString(const Field &field, Reflect* obj);
    [[nodiscard]] string convertVectorToJsonString(const Field &field, Reflect *obj);


    [[nodiscard]] static string vectorToString(const vector<char>& source);
    [[nodiscard]] static string vectorToString(const vector<string>& source);
    [[nodiscard]] string vectorToString(const vector<Reflect*>& source);

    template<typename U>
    [[nodiscard]] static string vectorToString(const vector<U>& source, string (*parseFunc)(U val)){

        if (source.empty())
            return "[]";

        string res = "[";
        for(U u : source){
            string s = parseFunc(u);
            res += s + ',';
        }
        res[res.size()-1] = ']';
        return res;
    }

    Reflect* ptr;
    vector<int>* vecInt = nullptr;
    vector<short>* vecShort = nullptr;
    vector<long>* vecLong = nullptr;
    vector<float>* vecFloat = nullptr;
    vector<double>* vecDouble = nullptr;
    vector<bool>* vecBool = nullptr;
    vector<char>* vecChar = nullptr;
    vector<string>* vecString = nullptr;
};


#endif //WINTER_JSONSERIALIZER_H
