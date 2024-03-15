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

    Reflect* ptr;
};


#endif //WINTER_JSONSERIALIZER_H
