//
// Created by Alienson on 27.1.2024.
//

#ifndef WINTER_LOGGER_TESTS_H
#define WINTER_LOGGER_TESTS_H

#include <vector>
#include <fstream>
#include "../include/log/Loggy.h"
#include "../vendor/include/catch.hpp"

//WARNING TESTS ARE LINE SENSITIVE SINCE LOGGER LOGS THE CURRENT LINE NUMBER
TEST_CASE("Logger Tests", "[Logger::info]"){

    auto* fout = new std::ofstream("winter.log");
    Loggy::getInstance()->addAppender(fout);
    std::ifstream fin("winter.log");

    std::string buffer;

    SECTION("Logging string") {
        wtLogInfo("Hello World");
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  lib/tests/loggerTests.h:19\tHello World");
    }

    SECTION("Logging String with parameters: int") {
        wtLogInfo("There are {} tests in this file", 5);
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  lib/tests/loggerTests.h:26\tThere are 5 tests in this file");
    }

    SECTION("Logging String with parameters: int, int") {
        wtLogInfo("There are {}/{} tests in this file", 10, 2);
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  lib/tests/loggerTests.h:33\tThere are 10/2 tests in this file");
    }

    SECTION("Logging String with parameters: float, double, char, long") {
        wtLogInfo("This is a float:{}, this is a double:{}, this is a char:{}, this is a long: {}", 1.2f, 2.68, 'c', 128l);
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  lib/tests/loggerTests.h:40\tThis is a float:1.2, this is a double:2.68, this is a char:c, this is a long: 128");
    }

    SECTION("Logging String with parameters: string") {
        wtLogInfo("Hello {}", "world");
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  lib/tests/loggerTests.h:47\tHello world");
    }

    SECTION("Logging String with parameters: vec") {
        wtLogInfo("Received vec: {}", std::vector<int>{1,2,3});
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  lib/tests/loggerTests.h:54\tReceived vec: [1,2,3]");
    }

    SECTION("Logging String with parameters: empty vec") {
        std::vector<int> vec{};
        wtLogInfo("Received vec: {}", vec);
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  lib/tests/loggerTests.h:62\tReceived vec: []");
    }

    SECTION("Logging String with parameters: array") {
        int array[3] = {1,2,3};
        wtLogInfo("Received array: {}", array);
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  lib/tests/loggerTests.h:70\tReceived array: [1,2,3]");
    }


    fout->close();
    fin.close();
}

void basicString(){
    const char* s = "Hello World";
    wtLogInfo(s);
}

void primitiveTypes(){
    const char* s = "There are {} tests in this file";
    wtLogInfo(s, 5);
}

//TODO
void primitiveTypes2(){
    const char* s = "This is a {} out of {} tests of primitive types";
    wtLogInfo(s, "second", 2);
    wtLogInfo(s, 2, 2);
}

void vectorTest(){
    std::vector<int> vec{1,2,3,4,5};
    wtLogInfo("5 Numbers:", vec);
}
void emptyVector(){
    std::vector<int> vec{};
    wtLogInfo("0 Numbers:", vec);
}
void arrayTest(){
    int array[]{1,2,3,4,5};
    wtLogInfo("Array", array,5);
}

#endif //WINTER_LOGGER_TESTS_H