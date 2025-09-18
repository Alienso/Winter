cmake_minimum_required(VERSION 3.16)
project(Winter)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -g -O0")
endif (WIN32)
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -g -O0 -fuse-ld=mold")
endif(UNIX)

#Postgres
if (UNIX)
    include_directories(/usr/include/postgresql/)
    link_directories(/usr/local/pgsql/lib/)
endif(UNIX)
if (WIN32)
    #include_directories("C:/Program Files/PostgreSQL/12/include")
    link_directories("C:/Program Files/PostgreSQL/12/lib")
endif(WIN32)

include_directories(../include)
include_directories(../vendor/include)
include_directories(../vendor/include/postgreSQL)

link_directories(../vendor/lib)

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