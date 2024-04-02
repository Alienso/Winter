//
// Created by alienson on 18.3.24..
//

#ifndef WINTER_SERIALIZATIONTESTS_H
#define WINTER_SERIALIZATIONTESTS_H

#include "Reflect.h"

#include "JsonSerializer.h"
#include "util/dtoUtils.h"


TEST_CASE("Serialization Tests", "[JsonSerializer::serialize]"){

    AllFieldsDTO dto;
    fillFieldData(dto);
    fillPtrData(dto);
    InnerClass clazz{5,2.5,"c"};

    std::cout << "FIELD COUNT: " << dto.getDeclaredFields().size() << '\n';


    SECTION("Serializing InnerClass"){
        string expected = "{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n}";
        string* s = JsonSerializer::serialize(&clazz);
        REQUIRE(*s == expected);
    }

    SECTION("Serializing AllFieldsDTO"){
        string expected = "{\n\"s\":1,\n\"i\":5,\n\"l\":10,\n\"f\":5.500000,\n\"d\":1.250000,\n\"c\":\"97\",\n\"str\":\"Hello World\",\n\"b\":false,\n\"bt\":\"\",\n\"sPtr\":1,\n\"iPtr\":5,\n\"lPtr\":10,\n\"fPtr\":5.500000,\n\"dPtr\":1.250000,\n\"cPtr\":\"97\",\n\"strPtr\":\"Hello World\",\n\"bPtr\":false,\n\"btPtr\":\"\",\n\"innerClassPtr\":{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},\n\"innerClass\":{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},\n\"vec\":[1,2,3],\n\"vecPtr\":[1,2,3]\n}";
        string* s = JsonSerializer::serialize(&dto);
        REQUIRE(*s == expected);
    }

}

TEST_CASE("Vector Serialization Tests", "[JsonSerializer::serialize]"){

    auto* allFieldsVecDto = new AllFieldsVecDTO();
    fillVectorFieldData(*allFieldsVecDto);
    fillVectorPtrData(*allFieldsVecDto);

    /*SECTION("Serializing empty vec-s") {
        string expected = "{\n\"i\":[],\n\"f\":[],\n\"s\":[],\n\"innerClassPtr\":[]\n}";
        string *s = JsonSerializer::serialize(allFieldsVecDto);
        REQUIRE(*s == expected);
    }*/

    SECTION("Serializing vec<Reflect*>") {
        string expected = "{\n\"s\":[1,2,3],\n\"i\":[5,4,3],\n\"l\":[10,15,20],\n\"f\":[5.500000,2.000000],\n\"d\":[1.250000,0.000000],\n\"c\":[\"a\",\"b\"],\n\"str\":[\"Hello\",\"World\"],\n\"b\":[false,true],\n\"bt\":\"\",\n\"sPtr\":[1,2,3],\n\"iPtr\":[5,4,3],\n\"lPtr\":[10,15,20],\n\"fPtr\":[5.500000,2.000000],\n\"dPtr\":[1.250000,0.000000],\n\"cPtr\":[\"a\",\"b\"],\n\"strPtr\":[\"Hello\",\"World\"],\n\"bPtr\":[false,true],\n\"btPtr\":\"\",\n\"innerClassPtr\":[{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},{\n\"x\":1.000000,\n\"y\":0.000000,\n\"c\":\"s\"\n}],\n\"innerClass\":[{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},{\n\"x\":1.000000,\n\"y\":0.000000,\n\"c\":\"s\"\n}],\n\"vecPtrInt\":[1,2,3],\n\"vecPtrIntPtr\":[0,0,7],\n\"vecPtrString\":[\"Hi\",\"World\"],\n\"vecPtrStringPtr\":[\"World\",\"Hi\"],\n\"vecPtrInnerClass\":[{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},{\n\"x\":1.000000,\n\"y\":0.000000,\n\"c\":\"s\"\n}],\n\"vecPtrInnerClassPtr\":[{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},{\n\"x\":1.000000,\n\"y\":0.000000,\n\"c\":\"s\"\n}]\n}";
        string *s = JsonSerializer::serialize(allFieldsVecDto);
        REQUIRE(*s == expected);
    }
}

#endif //WINTER_SERIALIZATIONTESTS_H