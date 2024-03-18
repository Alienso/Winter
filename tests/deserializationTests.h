//
// Created by alienson on 17.3.24..
//

#ifndef WINTER_DESERIALIZATIONTESTS_H
#define WINTER_DESERIALIZATIONTESTS_H

#endif //WINTER_DESERIALIZATIONTESTS_H

#include "./target/reflect/Reflect.h"

#include "./source/serialize/JsonDeserializer.h"
#include "./dto/AllFieldsDTO.h"
#include "./dto/BaseRequest.h"

Reflect* _AllFieldsDTO_(){
    return new AllFieldsDTO();
}

static int x = []{
    Reflect::initializeReflection();
    AllFieldsDTO::initializeReflection();
    Reflect::classMap["AllFieldsDTO"] = &_AllFieldsDTO_;
    return 0;
}();

TEST_CASE("Deserialization Tests", "[JsonDeserializer::deserialize]"){

    auto* request = new AllFieldsDTO();
    JsonDeserializer deserializer{};

    SECTION("Deserialization int"){
        string s = "{\"i\":5}";
        deserializer.deserialize(s, request);
        REQUIRE(request->i == 5);
    }

    SECTION("Deserialization float"){
        string s = "{\"f\":1.25}";
        deserializer.deserialize(s, request);
        REQUIRE(request->f == 1.25);
    }

    SECTION("Deserialization string"){
        string s = "{\"s\":\"Hello World\"}";
        deserializer.deserialize(s, request);
        REQUIRE(request->s == "Hello World");
    }

    SECTION("Deserialization array"){
        string s = "{\"vec\":[1,2,3]}";
        deserializer.deserialize(s, request);
        REQUIRE(request->vec == std::vector<int>{1,2,3});
    }

    SECTION("Deserialization object ptr"){
        string s = R"({"innerClassPtr":{"x":5.0, "y":2.5, "c":"c"}})";
        InnerClass clazz{5,2.5,"c"};
        deserializer.deserialize(s, request);
        REQUIRE(request->innerClassPtr->x == clazz.x);
        REQUIRE(request->innerClassPtr->y == clazz.y);
        REQUIRE(request->innerClassPtr->c == clazz.c);
    }

    SECTION("Deserialization allTypes"){
        string s = R"({"i":5, "f":5.5, "s": "Hello World", "vec": [1,2,3] ,"innerClassPtr":{"x":5.0, "y": 2.5, "c":"c"}})";
        AllFieldsDTO dto;
        dto.i = 5;
        dto.f = 5.5;
        dto.s = "Hello World";
        dto.vec = {1,2,3};
        InnerClass clazz{5,2.5,"c"};
        dto.innerClassPtr = &clazz;

        deserializer.deserialize(s, request);
        REQUIRE(request->i == 5);
        REQUIRE(request->f == 5.5);
        REQUIRE(request->s == "Hello World");
        REQUIRE(request->vec == std::vector<int>{1,2,3});
        REQUIRE(request->innerClassPtr->x == clazz.x);
        REQUIRE(request->innerClassPtr->y == clazz.y);
        REQUIRE(request->innerClassPtr->c == clazz.c);
    }

    SECTION("Deserialization spaces check"){
        string s = R"({"i" : 5, "f" :5.5,"s": "Hello World", "vec"  :  [1,2,3] ,"innerClassPtr":{ "x":5.0, "y": 2.5, "c":"c"  } })";
        AllFieldsDTO dto;
        dto.i = 5;
        dto.f = 5.5;
        dto.s = "Hello World";
        dto.vec = {1,2,3};
        InnerClass clazz{5,2.5,"c"};
        dto.innerClassPtr = &clazz;

        deserializer.deserialize(s, request);
        REQUIRE(request->i == 5);
        REQUIRE(request->f == 5.5);
        REQUIRE(request->s == "Hello World");
        REQUIRE(request->vec == std::vector<int>{1,2,3});
        REQUIRE(request->innerClassPtr->x == clazz.x);
        REQUIRE(request->innerClassPtr->y == clazz.y);
        REQUIRE(request->innerClassPtr->c == clazz.c);
    }
}