//
// Created by alienson on 17.3.24..
//

#ifndef WINTER_DESERIALIZATIONTESTS_H
#define WINTER_DESERIALIZATIONTESTS_H

#endif //WINTER_DESERIALIZATIONTESTS_H

#include "./target/reflect/Reflect.h"

#include "./source/serialize/JsonDeserializer.h"
#include "./dto/AllFieldsDTO.h"
#include "./dto/AllFieldsVecDTO.h"
#include "./dto/BaseRequest.h"

Reflect* _AllFieldsDTO_(){
    return new AllFieldsDTO();
}

Reflect* _AllFieldsVecDTO_(){
    return new AllFieldsVecDTO();
}

static int x = []{
    Reflect::initializeReflection();
    AllFieldsDTO::initializeReflection();
    AllFieldsVecDTO::initializeReflection();
    Reflect::classMap["AllFieldsDTO"] = &_AllFieldsDTO_;
    Reflect::classMap["AllFieldsVecDTO"] = &_AllFieldsVecDTO_;
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
        string s = R"({"s":"Hello World"})";
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

    SECTION("Deserialization allTypes with ptr"){
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

    SECTION("Deserialization allTypes with obj"){
        string s = R"({"i":5, "f":5.5, "s": "Hello World", "vec": [1,2,3] ,"innerClassObj":{"x":5.0, "y": 2.5, "c":"c"}})";
        AllFieldsDTO dto;
        dto.i = 5;
        dto.f = 5.5;
        dto.s = "Hello World";
        dto.vec = {1,2,3};
        InnerClass clazz{5,2.5,"c"};
        dto.innerClassObj = clazz;

        deserializer.deserialize(s, request);
        REQUIRE(request->i == 5);
        REQUIRE(request->f == 5.5);
        REQUIRE(request->s == "Hello World");
        REQUIRE(request->vec == std::vector<int>{1,2,3});
        REQUIRE(request->innerClassObj.x == clazz.x);
        REQUIRE(request->innerClassObj.y == clazz.y);
        REQUIRE(request->innerClassObj.c == clazz.c);
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

TEST_CASE("Vector Deserialization Tests", "[JsonDeserializer::deserialize]"){

    auto* request = new AllFieldsVecDTO();
    JsonDeserializer deserializer{};

    SECTION("Deserialization of vec<int>"){
        string s = R"({"i":[1,2,3,4,5]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->i == std::vector<int>{1,2,3,4,5});
    }

    SECTION("Deserialization of vec<float>"){
        string s = R"({"f":[1.5,2.25,3.125,4.0,5.667]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->f == std::vector<float>{1.5f,2.25f,3.125f,4.0f,5.667f});
    }

    SECTION("Deserialization of vec<string>"){
        string s = R"({"s":["Hello","World"]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->s[0] == "Hello");
        REQUIRE(request->s[1] == "World");
    }

    SECTION("Deserialization of vec<Reflect*>"){
        string s = R"({"innerClassPtr":[{"x":1.0, "y": 2.0, "c":"char"},{"x":5.5, "y": 8.5, "c":"str"}]})";

        auto* innerClass1 = new InnerClass(1.0f,2.0f,"char");
        auto* innerClass2 = new InnerClass(5.5f,8.5f,"str");

        deserializer.deserialize(s,request);

        REQUIRE(request->innerClassPtr[0]->x == innerClass1->x);
        REQUIRE(request->innerClassPtr[0]->y == innerClass1->y);
        REQUIRE(request->innerClassPtr[0]->c == innerClass1->c);
        REQUIRE(request->innerClassPtr[1]->x == innerClass2->x);
        REQUIRE(request->innerClassPtr[1]->y == innerClass2->y);
        REQUIRE(request->innerClassPtr[1]->c == innerClass2->c);
    }

}