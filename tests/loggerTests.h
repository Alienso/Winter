//
// Created by Alienson on 27.1.2024..
//

#include <vector>
#include "../source/log/Logger.h"

using namespace std;

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

void runLoggerTests(){
    basicString();
    primitiveTypes();
    primitiveTypes2();
    vectorTest();
    emptyVector();
    arrayTest();
}