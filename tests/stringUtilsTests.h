//
// Created by alienson on 27.3.24..
//

#ifndef WINTER_STRINGUTILSTESTS_H
#define WINTER_STRINGUTILSTESTS_H

#endif //WINTER_STRINGUTILSTESTS_H

#include "stringUtils.h"

using namespace std;

//TODO

TEST_CASE("StringUtils trim Tests", "[trim]"){

    SECTION("trim string that can be trimmed on both sides"){
        string s = " HelloWorld ";
        REQUIRE("HelloWorld" == StringUtils::trim(s));
    }

    SECTION("trim string that can be trimmed on left side"){
        string s = "\t HelloWorld";
        REQUIRE("HelloWorld" == StringUtils::trim(s));
    }

    SECTION("trim string that can be trimmed on right side"){
        string s = "HelloWorld\n\n ";
        REQUIRE("HelloWorld" == StringUtils::trim(s));
    }

    SECTION("trim empty string"){
        string s = " \n\t";
        REQUIRE(StringUtils::trim(s).empty());
    }
}

TEST_CASE("StringUtils toCamelCase Tests", "[toCamelCase]"){
    SECTION("string with _"){
        string s = "created_on";
        REQUIRE("createdOn" == StringUtils::toCamelCase(s));
    }

    SECTION("string with ' '"){
        string s = "created on";
        REQUIRE("createdOn" == StringUtils::toCamelCase(s));
    }

    SECTION("string with _ and upperCase"){
        string s = "created_On";
        REQUIRE("createdOn" == StringUtils::toCamelCase(s));
    }

    SECTION("string with ' ' and upperCase"){
        string s = "created On";
        REQUIRE("createdOn" == StringUtils::toCamelCase(s));
    }

    SECTION("regular string"){
        string s = "created";
        REQUIRE("created" == StringUtils::toCamelCase(s));
    }

    SECTION("empty string"){
        string s = "";
        REQUIRE( StringUtils::toCamelCase(s).empty() );
    }

    SECTION("uppercaseString"){
        string s = "CREATED_ON";
        REQUIRE("CREATEDON" == StringUtils::toCamelCase(s));
    }

    SECTION("string ends with _"){
        string s = "created_";
        REQUIRE("created_" == StringUtils::toCamelCase(s));
    }

    SECTION("string ends with ' '"){
        string s = "created ";
        REQUIRE("created " == StringUtils::toCamelCase(s));
    }

    SECTION("string starts with _"){
        string s = "_created";
        REQUIRE("Created" == StringUtils::toCamelCase(s));
    }

    SECTION("string starts with __"){
        string s = "__created";
        REQUIRE("_Created" == StringUtils::toCamelCase(s));
    }

    SECTION("string ends with __"){
        string s = "created__";
        REQUIRE("created__" == StringUtils::toCamelCase(s));
    }
}