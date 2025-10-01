//
// Created by alienson on 28.3.24.
//

#ifndef WINTER_REFLECTTESTS_H
#define WINTER_REFLECTTESTS_H

#include "../vendor/include/catch.hpp"
#include "util/dtoUtils.h"


TEST_CASE("Field copy value", "Field::copyValue"){

    auto* source = new AllFieldsDTO();
    fillFieldData(*source);
    fillPtrData(*source);

    auto* dest = new AllFieldsDTO();

    SECTION("copy value short -> short") {
        Field::copyValue(source, *source->getField("s"), dest, *dest->getField("s"));
        REQUIRE(dest->s == 1);
    }

    SECTION("copy values int -> int") {
        Field::copyValue(source, *source->getField("i"), dest, *dest->getField("i"));
        REQUIRE(dest->i == 5);
    }

    SECTION("copy values long -> long") {
        Field::copyValue(source, *source->getField("l"), dest, *dest->getField("l"));
        REQUIRE(dest->l == 10);
    }

    SECTION("copy values float -> float") {
        Field::copyValue(source, *source->getField("f"), dest, *dest->getField("f"));
        REQUIRE(dest->f == 5.5f);
    }

    SECTION("copy values double -> double") {
        Field::copyValue(source, *source->getField("d"), dest, *dest->getField("d"));
        REQUIRE(dest->d == 1.25);
    }

    SECTION("copy values bool -> bool") {
        Field::copyValue(source, *source->getField("b"), dest, *dest->getField("b"));
        REQUIRE(dest->b == false);
    }

    SECTION("copy values char -> char") {
        Field::copyValue(source, *source->getField("c"), dest, *dest->getField("c"));
        REQUIRE(dest->c == 'a');
    }

    SECTION("copy values str -> str") {
        Field::copyValue(source, *source->getField("str"), dest, *dest->getField("str"));
        REQUIRE(dest->str == "Hello World");
    }

    SECTION("copy values vecInt -> vecInt") {
        Field::copyValue(source, *source->getField("vec"), dest, *dest->getField("vec"));
        REQUIRE(dest->vec == std::vector<int>{1,2,3});
    }

    SECTION("copy values vecInt -> vecInt") {
        Field::copyValue(source, *source->getField("innerClass"), dest, *dest->getField("innerClass"));
        REQUIRE(dest->innerClass.x == 5);
        REQUIRE(dest->innerClass.y == 2.5);
        REQUIRE(dest->innerClass.c == "c");
    }
}



TEST_CASE("Field copy vector value", "Field::copyValue"){

    auto* source = new AllFieldsVecDTO();
    fillVectorFieldData(*source);
    fillVectorPtrData(*source);

    auto* dest = new AllFieldsVecDTO();

    SECTION("copy vector value short -> short") {
        Field::copyValue(source, *source->getField("s"), dest, *dest->getField("s"));
        REQUIRE(dest->s[0] == 1);
        REQUIRE(dest->s[1] == 2);
        REQUIRE(dest->s[2] == 3);
    }

    SECTION("copy values int -> int") {
        Field::copyValue(source, *source->getField("i"), dest, *dest->getField("i"));
        REQUIRE(dest->i[0] == 5);
        REQUIRE(dest->i[1] == 4);
        REQUIRE(dest->i[2] == 3);
    }

    SECTION("copy values long -> long") {
        Field::copyValue(source, *source->getField("l"), dest, *dest->getField("l"));
        REQUIRE(dest->l[0] == 10);
        REQUIRE(dest->l[1] == 15);
        REQUIRE(dest->l[2] == 20);
    }

    SECTION("copy values float -> float") {
        Field::copyValue(source, *source->getField("f"), dest, *dest->getField("f"));
        REQUIRE(dest->f[0] == 5.5f);
        REQUIRE(dest->f[1] == 2.0f);
    }

    SECTION("copy values double -> double") {
        Field::copyValue(source, *source->getField("d"), dest, *dest->getField("d"));
        REQUIRE(dest->d[0] == 1.25);
        REQUIRE(dest->d[1] == 0.0);
    }

    SECTION("copy values bool -> bool") {
        Field::copyValue(source, *source->getField("b"), dest, *dest->getField("b"));
        REQUIRE(dest->b[0] == false);
        REQUIRE(dest->b[1] == true);
    }

    SECTION("copy values char -> char") {
        Field::copyValue(source, *source->getField("c"), dest, *dest->getField("c"));
        REQUIRE(dest->c[0] == 'a');
        REQUIRE(dest->c[1] == 'b');
    }

    SECTION("copy values str -> str") {
        Field::copyValue(source, *source->getField("str"), dest, *dest->getField("str"));
        REQUIRE(dest->str[0] == "Hello");
        REQUIRE(dest->str[0] == "World");
    }

    SECTION("copy values vecInt -> vecInt") {
        Field::copyValue(source, *source->getField("innerClass"), dest, *dest->getField("innerClass"));
        REQUIRE(dest->innerClass[0].x == 5);
        REQUIRE(dest->innerClass[0].y == 2.5);
        REQUIRE(dest->innerClass[0].c == "c");

        REQUIRE(dest->innerClass[1].x == 1);
        REQUIRE(dest->innerClass[1].y == 0.0);
        REQUIRE(dest->innerClass[1].c == "s");
    }
}


#endif //WINTER_REFLECTTESTS_H