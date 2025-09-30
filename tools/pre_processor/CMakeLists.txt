cmake_minimum_required(VERSION 3.16)
project(PreProcessor)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -g -O0")
endif (WIN32)
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -g -O0")
endif(UNIX)

include_directories(../../include)

add_executable(PreProcessor main.cpp
        PreProcessor.cpp PreProcessor.h
        pass/ReflectionPass.cpp pass/ReflectionPass.h
        pass/Pass.cpp pass/Pass.h
        pass/AnnotationPass.cpp pass/AnnotationPass.h
        pass/ComponentPass.cpp pass/ComponentPass.h
        util.h)