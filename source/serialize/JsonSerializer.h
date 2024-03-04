//
// Created by Alienson on 14.2.2024..
//

#ifndef WINTER_JSONSERIALIZER_H
#define WINTER_JSONSERIALIZER_H

#include "../reflect/Reflect.h"
#include "../log/Logger.h"
#include "FieldEnums.h"
#include "../util/stringUtils.h"

using namespace std;

class JsonSerializer {
public:
    string* serialize (Reflect* obj){
        auto* s = new string();
        for (auto field : Reflect::getDeclaredFields()){
            ;;
        }
        return s;
    }
};


#endif //WINTER_JSONSERIALIZER_H
