//
// Created by alienson on 27.3.24.
//

#ifndef WINTER_STRINGUTILSTESTS_H
#define WINTER_STRINGUTILSTESTS_H

#include "/util/stringUtils.h"

//TODO

TEST_CASE("StringUtils trim Tests", "[trim]"){

    SECTION("trim string that can be trimmed on both sides"){
        std::string s = " HelloWorld ";
        REQUIRE("HelloWorld" == StringUtils::trim(s));
    }

    SECTION("trim string that can be trimmed on left side"){
        std::string s = "\t HelloWorld";
        REQUIRE("HelloWorld" == StringUtils::trim(s));
    }

    SECTION("trim string that can be trimmed on right side"){
        std::string s = "HelloWorld\n\n ";
        REQUIRE("HelloWorld" == StringUtils::trim(s));
    }

    SECTION("trim empty string"){
        std::string s = " \n\t";
        REQUIRE(StringUtils::trim(s).empty());
    }
}

TEST_CASE("StringUtils toCamelCase Tests", "[toCamelCase]"){
    SECTION("string with _"){
        std::string s = "created_on";
        REQUIRE("createdOn" == StringUtils::toCamelCase(s));
    }

    SECTION("string with ' '"){
        std::string s = "created on";
        REQUIRE("createdOn" == StringUtils::toCamelCase(s));
    }

    SECTION("string with _ and upperCase"){
        std::string s = "created_On";
        REQUIRE("createdOn" == StringUtils::toCamelCase(s));
    }

    SECTION("string with ' ' and upperCase"){
        std::string s = "created On";
        REQUIRE("createdOn" == StringUtils::toCamelCase(s));
    }

    SECTION("regular string"){
        std::string s = "created";
        REQUIRE("created" == StringUtils::toCamelCase(s));
    }

    SECTION("empty string"){
        std::string s = "";
        REQUIRE( StringUtils::toCamelCase(s).empty() );
    }

    SECTION("uppercaseString"){
        std::string s = "CREATED_ON";
        REQUIRE("CREATEDON" == StringUtils::toCamelCase(s));
    }

    SECTION("string ends with _"){
        std::string s = "created_";
        REQUIRE("created_" == StringUtils::toCamelCase(s));
    }

    SECTION("string ends with ' '"){
        std::string s = "created ";
        REQUIRE("created " == StringUtils::toCamelCase(s));
    }

    SECTION("string starts with _"){
        std::string s = "_created";
        REQUIRE("Created" == StringUtils::toCamelCase(s));
    }

    SECTION("string starts with __"){
        std::string s = "__created";
        REQUIRE("_Created" == StringUtils::toCamelCase(s));
    }

    SECTION("string ends with __"){
        std::string s = "created__";
        REQUIRE("created__" == StringUtils::toCamelCase(s));
    }
}

#endif //WINTER_STRINGUTILSTESTS_H