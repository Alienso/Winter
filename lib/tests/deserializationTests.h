//
// Created by alienson on 17.3.24.
//

#ifndef WINTER_DESERIALIZATIONTESTS_H
#define WINTER_DESERIALIZATIONTESTS_H

#include "reflect/Reflect.h"

#include "../../include/serialize/JsonDeserializer.h"
#include "./dto/AllFieldsDTO.h"
#include "./dto/AllFieldsVecDTO.h"
#include "./dto/BaseRequest.h"

//TODO add type or name missMatch cases

TEST_CASE("Deserialization Tests", "[JsonDeserializer::deserialize]"){

    auto* request = new AllFieldsDTO();
    JsonDeserializer deserializer{};

    SECTION("Deserialization allTypes"){
        std::string s = R"({"s":1, "l":2, "i":5, "f":5.5, "d":1.5, "b": true, "str": "Hello World", "vec": [1,2,3], "innerClass":{"x":7.5, "y": 0.5, "c":"str"}, "innerClassPtr":{"x":5.0, "y": 2.5, "c":"c"}})";
        deserializer.deserialize(s, request);

        REQUIRE(request->s == 1);
        REQUIRE(request->i == 5);
        REQUIRE(request->l == 2);
        REQUIRE(request->f == 5.5);
        REQUIRE(request->d == 1.5);
        REQUIRE(request->b);
        REQUIRE(request->str == "Hello World");
        REQUIRE(request->vec == std::vector<int>{1,2,3});
        REQUIRE(request->innerClass.x == 7.5);
        REQUIRE(request->innerClass.y == 0.5);
        REQUIRE(request->innerClass.c == "str");
        REQUIRE(request->innerClassPtr->x == 5);
        REQUIRE(request->innerClassPtr->y == 2.5);
        REQUIRE(request->innerClassPtr->c == "c");
    }

    SECTION("Deserialization allTypes Ptr"){
        std::string s = R"({"sPtr":1, "lPtr":2, "iPtr":5, "fPtr":5.5, "dPtr":1.5, "bPtr": true, "strPtr": "Hello World", "vecPtr": [1,2,3]})";
        deserializer.deserialize(s, request);

        REQUIRE(*request->sPtr == 1);
        REQUIRE(*request->iPtr == 5);
        REQUIRE(*request->lPtr == 2);
        REQUIRE(*request->fPtr == 5.5);
        REQUIRE(*request->dPtr == 1.5);
        REQUIRE(*request->bPtr);
        REQUIRE(*request->strPtr == "Hello World");
        REQUIRE(*request->vecPtr == std::vector<int>{1,2,3});
    }

    SECTION("Deserialization float/double natural number check"){
        std::string s = R"({"f": 1, "d": 5})";
        deserializer.deserialize(s, request);

        REQUIRE(request->f == 1.0f);
        REQUIRE(request->d == 5.0);
    }

    SECTION("Deserialization spaces check"){
        std::string s = R"({"i" : 5, "f" :5.5,"str": "Hello World", "vec"  :  [1,2,3] ,"innerClassPtr":{ "x":5.0, "y": 2.5, "c":"c"  } })";

        deserializer.deserialize(s, request);
        REQUIRE(request->i == 5);
        REQUIRE(request->f == 5.5);
        REQUIRE(request->str == "Hello World");
        REQUIRE(request->vec == std::vector<int>{1,2,3});
        REQUIRE(request->innerClassPtr->x == 5);
        REQUIRE(request->innerClassPtr->y == 2.5);
        REQUIRE(request->innerClassPtr->c == "c");
    }
}

TEST_CASE("Vector Deserialization Tests", "[JsonDeserializer::deserialize]"){

    auto* request = new AllFieldsVecDTO();
    JsonDeserializer deserializer{};

    SECTION("Deserialization of vec<short>"){
        std::string s = R"({"s":[1,2,8,4,5]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->s == std::vector<short>{1,2,8,4,5});
    }

    SECTION("Deserialization of vec<int>"){
        std::string s = R"({"i":[1,2,3,4,5]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->i == std::vector<int>{1,2,3,4,5});
    }

    SECTION("Deserialization of vec<long>"){
        std::string s = R"({"l":[5,4,3,2,1]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->l == std::vector<long>{5,4,3,2,1});
    }

    SECTION("Deserialization of vec<float>"){
        std::string s = R"({"f":[1.5,2.25,3.125,4.0,5.667]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->f == std::vector<float>{1.5f,2.25f,3.125f,4.0f,5.667f});
    }

    SECTION("Deserialization of vec<double>"){
        std::string s = R"({"d":[1,2.25,3.125,4.0,5.667]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->d == std::vector<double>{1.0,2.25,3.125,4.0,5.667});
    }

    SECTION("Deserialization of vec<string>"){
        std::string s = R"({"str":["Hello","World"]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->str[0] == "Hello");
        REQUIRE(request->str[1] == "World");
    }

    SECTION("Deserialization of vec<bool>"){
        std::string s = R"({"b":[true, false, false]})";
        deserializer.deserialize(s,request);
        REQUIRE(request->b == std::vector<bool>{true, false, false});
    }

    SECTION("Deserialization of vec<InnerClass>"){
        std::string s = R"({"innerClass":[{"x":1.0, "y": 2.0, "c":"char"},{"x":5.5, "y": 8.5, "c":"str"}]})";
        deserializer.deserialize(s,request);

        REQUIRE(request->innerClass[0].x == 1.0);
        REQUIRE(request->innerClass[0].y == 2.0);
        REQUIRE(request->innerClass[0].c == "char");
        REQUIRE(request->innerClass[1].x == 5.5);
        REQUIRE(request->innerClass[1].y == 8.5);
        REQUIRE(request->innerClass[1].c == "str");
    }

    SECTION("Deserialization of vec<Reflect*>"){
        std::string s = R"({"innerClassPtr":[{"x":1.0, "y": 2.0, "c":"char"},{"x":5.5, "y": 8.5, "c":"str"}]})";
        deserializer.deserialize(s,request);

        REQUIRE(request->innerClassPtr[0]->x == 1.0);
        REQUIRE(request->innerClassPtr[0]->y == 2.0);
        REQUIRE(request->innerClassPtr[0]->c == "char");
        REQUIRE(request->innerClassPtr[1]->x == 5.5);
        REQUIRE(request->innerClassPtr[1]->y == 8.5);
        REQUIRE(request->innerClassPtr[1]->c == "str");
    }

}


template<typename T>
bool areVectorsEqual(std::vector<T*> vec, std::vector<T>& other){
    if (vec.size() != other.size())
        return false;
    for(size_t i=0; i<vec.size(); i++){
        if (*(vec[i]) != other[i])
            return false;
    }
    return true;
}

TEST_CASE("Vector Ptr Deserialization Tests", "[JsonDeserializer::deserialize]"){

    auto* request = new AllFieldsVecDTO();
    JsonDeserializer deserializer{};

    SECTION("Deserialization of vec<short*>"){
        std::string s = R"({"sPtr":[1,2,8,4,5]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<short>{1,2,8,4,5};
        REQUIRE(areVectorsEqual(request->sPtr, vec));
    }

    SECTION("Deserialization of vec<int*>"){
        std::string s = R"({"iPtr":[1,2,3,4,5]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<int>{1,2,3,4,5};
        REQUIRE(areVectorsEqual(request->iPtr, vec));
    }

    SECTION("Deserialization of vec<long*>"){
        std::string s = R"({"lPtr":[5,4,3,2,1]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<long>{5,4,3,2,1};
        REQUIRE(areVectorsEqual(request->lPtr, vec));
    }

    SECTION("Deserialization of vec<float*>"){
        std::string s = R"({"fPtr":[1.5,2.25,3.125,4.0,5.667]})";
        deserializer.deserialize(s,request);

        auto vec =  std::vector<float>{1.5f,2.25f,3.125f,4.0f,5.667f};
        REQUIRE(areVectorsEqual(request->fPtr, vec));
    }

    SECTION("Deserialization of vec<double*>"){
        std::string s = R"({"dPtr":[1,2.25,3.125,4.0,5.667]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<double>{1.0,2.25,3.125,4.0,5.667};
        REQUIRE(areVectorsEqual(request->dPtr, vec));
    }

    SECTION("Deserialization of vec<string*>"){
        std::string s = R"({"strPtr":["Hello","World"]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<std::string>{"Hello", "World"};
        REQUIRE(areVectorsEqual(request->strPtr, vec));
    }

    SECTION("Deserialization of vec<bool*>"){
        std::string s = R"({"bPtr":[true, false, false]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<bool>{true, false, false};
        REQUIRE(areVectorsEqual(request->bPtr, vec));
    }

    SECTION("Deserialization of vec<Reflect*>"){
        std::string s = R"({"innerClassPtr":[{"x":1.0, "y": 2.0, "c":"char"},{"x":5.5, "y": 8.5, "c":"str"}]})";
        deserializer.deserialize(s,request);

        REQUIRE(request->innerClassPtr[0]->x == 1.0);
        REQUIRE(request->innerClassPtr[0]->y == 2.0);
        REQUIRE(request->innerClassPtr[0]->c == "char");
        REQUIRE(request->innerClassPtr[1]->x == 5.5);
        REQUIRE(request->innerClassPtr[1]->y == 8.5);
        REQUIRE(request->innerClassPtr[1]->c == "str");
    }

}


TEST_CASE("Vector Ptr Ptr Deserialization Tests", "[JsonDeserializer::deserialize]") {

    auto *request = new AllFieldsVecDTO();
    JsonDeserializer deserializer{};

    SECTION("Deserialization of vec<int>*"){
        std::string s = R"({"vecPtrInt":[1,2,8,4,5]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<int>{1,2,8,4,5};
        REQUIRE(*request->vecPtrInt == vec);
    }

    SECTION("Deserialization of vec<int*>*"){
        std::string s = R"({"vecPtrIntPtr":[1,2,8,4,5]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<int>{1,2,8,4,5};
        REQUIRE(areVectorsEqual(*request->vecPtrIntPtr, vec));
    }

    SECTION("Deserialization of vec<std::string>*"){
        std::string s = R"({"vecPtrString":["Hello", "World"]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<std::string>{"Hello", "World"};
        REQUIRE(*request->vecPtrString == vec);
    }

    SECTION("Deserialization of vec<string*>*"){
        std::string s = R"({"vecPtrStringPtr":["Hello", "World"]})";
        deserializer.deserialize(s,request);

        auto vec = std::vector<std::string>{"Hello", "World"};
        REQUIRE(areVectorsEqual(*request->vecPtrStringPtr, vec));
    }

    SECTION("Deserialization of vec<string>*"){
        std::string s = R"({"vecPtrInnerClass":[{"x":1.0, "y": 2.0, "c":"char"},{"x":5.5, "y": 8.5, "c":"str"}]})";
        deserializer.deserialize(s,request);

        REQUIRE((*request->vecPtrInnerClass)[0].x == 1.0);
        REQUIRE((*request->vecPtrInnerClass)[0].y == 2.0);
        REQUIRE((*request->vecPtrInnerClass)[0].c == "char");
        REQUIRE((*request->vecPtrInnerClass)[1].x == 5.5);
        REQUIRE((*request->vecPtrInnerClass)[1].y == 8.5);
        REQUIRE((*request->vecPtrInnerClass)[1].c == "str");
    }

    SECTION("Deserialization of vec<string*>*"){
        std::string s = R"({"vecPtrInnerClassPtr":[{"x":1.0, "y": 2.0, "c":"char"},{"x":5.5, "y": 8.5, "c":"str"}]})";
        deserializer.deserialize(s,request);

        REQUIRE((*request->vecPtrInnerClassPtr)[0]->x == 1.0);
        REQUIRE((*request->vecPtrInnerClassPtr)[0]->y == 2.0);
        REQUIRE((*request->vecPtrInnerClassPtr)[0]->c == "char");
        REQUIRE((*request->vecPtrInnerClassPtr)[1]->x == 5.5);
        REQUIRE((*request->vecPtrInnerClassPtr)[1]->y == 8.5);
        REQUIRE((*request->vecPtrInnerClassPtr)[1]->c == "str");
    }


}

#endif //WINTER_DESERIALIZATIONTESTS_H