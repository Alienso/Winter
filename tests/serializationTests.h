//
// Created by alienson on 18.3.24..
//

#ifndef WINTER_SERIALIZATIONTESTS_H
#define WINTER_SERIALIZATIONTESTS_H

#include "./target/reflect/Reflect.h"

#include "./source/serialize/JsonSerializer.h"
#include "./dto/AllFieldsDTO.h"
#include "./dto/BaseRequest.h"

/*Reflect* _AllFieldsDTO_(){
    return new AllFieldsDTO();
}

static int x = []{
    Reflect::initializeReflection();
    AllFieldsDTO::initializeReflection();
    Reflect::classMap["AllFieldsDTO"] = &_AllFieldsDTO_;
    return 0;
}();*/

TEST_CASE("Serialization Tests", "[JsonSerializer::serialize]"){

    JsonSerializer serializer{};

    AllFieldsDTO dto;
    dto.i = 5;
    dto.f = 5.5;
    dto.s = "Hello World";
    dto.vec = {1,2,3};
    InnerClass clazz{5,2.5,"c"};
    dto.innerClassPtr = &clazz;
    dto.innerClassObj = clazz;

    SECTION("Serializing InnerClass"){
        string expected = "{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n}";
        string* s = serializer.serialize(&clazz);
        REQUIRE(*s == expected);
    }

    SECTION("Serializing AllFieldsDTO"){
        string expected = "{\n\"i\":5,\n\"f\":5.500000,\n\"s\":\"Hello World\",\n\"vec\":[1,2,3],\n\"innerClassPtr\":{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},\n\"innerClassObj\":{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n}\n}";
        string* s = serializer.serialize(&dto);
        REQUIRE(*s == expected);
    }

    SECTION("Serializing AllFieldsDTO without ptr"){
        dto.innerClassPtr = nullptr;
        string expected = "{\n\"i\":5,\n\"f\":5.500000,\n\"s\":\"Hello World\",\n\"vec\":[1,2,3],\n\"innerClassPtr\":null,\n\"innerClassObj\":{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n}\n}";
        string* s = serializer.serialize(&dto);
        REQUIRE(*s == expected);
    }

}

TEST_CASE("Vector Serialization Tests", "[JsonSerializer::serialize]"){

    JsonSerializer serializer{};
    auto* allFieldsVecDto = new AllFieldsVecDTO();

    SECTION("Serializing empty vec-s") {
        string expected = "{\n\"i\":[],\n\"f\":[],\n\"s\":[],\n\"innerClassPtr\":[]\n}";
        string *s = serializer.serialize(allFieldsVecDto);
        REQUIRE(*s == expected);
    }

    SECTION("Serializing vec<int>") {
        allFieldsVecDto->i = {1,2,3};
        string expected = "{\n\"i\":[1,2,3],\n\"f\":[],\n\"s\":[],\n\"innerClassPtr\":[]\n}";
        string *s = serializer.serialize(allFieldsVecDto);
        REQUIRE(*s == expected);
    }

    SECTION("Serializing vec<float>") {
        allFieldsVecDto->f = {1.0f,2.5f,0.5f};
        string expected = "{\n\"i\":[],\n\"f\":[1.000000,2.500000,0.500000],\n\"s\":[],\n\"innerClassPtr\":[]\n}";
        string *s = serializer.serialize(allFieldsVecDto);
        REQUIRE(*s == expected);
    }

    SECTION("Serializing vec<string>") {
        allFieldsVecDto->s = {"Hello", "world"};
        string expected = "{\n\"i\":[],\n\"f\":[],\n\"s\":[\"Hello\",\"world\"],\n\"innerClassPtr\":[]\n}";
        string *s = serializer.serialize(allFieldsVecDto);
        REQUIRE(*s == expected);
    }

    SECTION("Serializing vec<Reflect*>") {
        auto* clazz1 = new InnerClass(5,2.5,"c");
        auto* clazz2 = new InnerClass(0,0.125,"SA");
        allFieldsVecDto->innerClassPtr = {clazz1, clazz2};
        string expected = "{\n\"i\":[],\n\"f\":[],\n\"s\":[],\n\"innerClassPtr\":[{\n\"x\":5.000000,\n\"y\":2.500000,\n\"c\":\"c\"\n},{\n\"x\":0.000000,\n\"y\":0.125000,\n\"c\":\"SA\"\n}]\n}";
        string *s = serializer.serialize(allFieldsVecDto);
        REQUIRE(*s == expected);
    }
}

#endif //WINTER_SERIALIZATIONTESTS_H