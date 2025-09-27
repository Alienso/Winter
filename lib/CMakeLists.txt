cmake_minimum_required(VERSION 3.16)
project(WinterLib)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -g -O0")
endif (WIN32)
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -g -O0 -fuse-ld=mold")
endif(UNIX)

include_directories(../include)
include_directories(../vendor/include)
include_directories(../vendor/include/postgreSQL)

link_directories(../vendor/lib)
link_directories(../vendor/lib/postgres)

#temp lib includes
include_directories(../include/log)
include_directories(../include/reflect)
include_directories(../include/serialize)
include_directories(../include/util)
include_directories(../include/http)
include_directories(../include/core)
include_directories(../include/sql)
include_directories(../include/util)

file(GLOB_RECURSE GENERATED_SOURCES "source/*.cpp")
add_library(Winter STATIC ${GENERATED_SOURCES})

if (WIN32)
    target_link_libraries(Winter wsock32 ws2_32 pq)
endif (WIN32)
if (UNIX)
    target_link_libraries(Winter pq)
endif(UNIX)

add_executable(WinterTests tests/runTests.cpp ${GENERATED_SOURCES})

if (WIN32)
    target_link_libraries(WinterTests wsock32 ws2_32 pq)
endif (WIN32)
if (UNIX)
    target_link_libraries(WinterTests pq)
endif(UNIX)
