//
// Created by Alienson on 27.1.2024..
//

#include <vector>
#include "../source/log/Logger.h"

using namespace std;
//WARNING TESTS ARE LINE SENSITIVE SINCE LOGGER LOGS THE CURRENT LINE NUMBER
TEST_CASE("Logger Tests", "[Logger::info]"){

    auto* fout = new ofstream("winter.log");
    Logger::getInstance()->addAppender(fout);
    ifstream fin("winter.log");

    string buffer;

    SECTION("Logging string") {
        wtLogInfo("Hello World");
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  tests/loggerTests.h:19\tHello World");
    }

    SECTION("Logging String with parameters: int") {
        wtLogInfo("There are %d tests in this file", 5);
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  tests/loggerTests.h:26\tThere are 5 tests in this file");
    }

    SECTION("Logging String with parameters: int, int") {
        wtLogInfo("There are %d/%d tests in this file", 10, 2);
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  tests/loggerTests.h:33\tThere are 10/2 tests in this file");
    }

    SECTION("Logging String with parameters: float, double, char, long") {
        wtLogInfo("This is a float:%f, this is a double:%e, this is a char:%c, this is a long: %ld", 1.2f, 2.68, 'c', 128l);
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  tests/loggerTests.h:40\tThis is a float:1.2, this is a double:2.68, this is a char:c, this is a long: 128");
    }

    SECTION("Logging String with parameters: string") {
        wtLogInfo("Hello %s", "world");
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  tests/loggerTests.h:47\tHello world");
    }

    SECTION("Logging String with parameters: vec") {
        wtLogInfo("Received vec: ", std::vector<int>{1,2,3});
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  tests/loggerTests.h:54\tReceived vec: [1,2,3]");
    }

    SECTION("Logging String with parameters: empty vec") {
        std::vector<int> vec{};
        wtLogInfo("Received vec: ", vec);
        fout->flush();
        getline(fin, buffer);
        //REQUIRE(buffer == "INFO:  tests/loggerTests.h:62\tReceived vec: []"); TODO??
    }

    SECTION("Logging String with parameters: array") {
        int array[3] = {1,2,3};
        wtLogInfo("Received array: ", array, 3);
        fout->flush();
        getline(fin, buffer);
        REQUIRE(buffer == "INFO:  tests/loggerTests.h:70\tReceived array: [1,2,3]");
    }


    fout->close();
    fin.close();
}

void basicString(){
    const char* s = "Hello World";
    wtLogInfo(s);
}

void primitiveTypes(){
    const char* s = "There are %d tests in this file";
    wtLogInfo(s, 5);
}

//TODO
void primitiveTypes2(){
    const char* s = "This is a %d out of %d tests of primitive types";
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