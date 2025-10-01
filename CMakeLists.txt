cmake_minimum_required(VERSION 3.16)
project(Winter)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(lib)
add_subdirectory(tools/pre_processor)
add_subdirectory(app)


if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -g -O0")
endif (WIN32)
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -g -O0")
endif(UNIX)


include_directories(include)
include_directories(vendor/include)
include_directories(vendor/include/postgreSQL)

link_directories(vendor/lib)
link_directories(vendor/lib/postgres)

include_directories(include/log)
include_directories(include/reflect)
include_directories(include/serialize)
include_directories(include/util)
include_directories(include/http)
include_directories(include/core)
include_directories(include/sql)
include_directories(include/util)

set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
set(LIB_SOURCES_DIR ${CMAKE_SOURCE_DIR}/lib/source)

if (NOT EXISTS ${GENERATED_DIR}/Component.cpp)
    execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/app/template/Component.cpp ${GENERATED_DIR}/Component.cpp
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/app/template/Reflect.cpp ${GENERATED_DIR}/Reflect.cpp
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/app/template/Router.cpp ${GENERATED_DIR}/Router.cpp
    )
endif()

add_custom_target(run_preprocessor
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
        COMMAND ${CMAKE_BINARY_DIR}/bin/PreProcessor ${CMAKE_SOURCE_DIR}/app/source ${GENERATED_DIR}
        DEPENDS ${CMAKE_BINARY_DIR}/bin/PreProcessor
        COMMENT "Running PreProcessor..."
)

execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/app/resources ${CMAKE_BINARY_DIR}/resources
)

add_executable(app ${CMAKE_SOURCE_DIR}/app/main.cpp)

# Collect all generated .cpp files after preprocessing
file(GLOB_RECURSE GENERATED_SOURCES "${GENERATED_DIR}/*.cpp")
target_sources(app PRIVATE ${GENERATED_SOURCES} )

#Use this for rebuild with new files
#add_dependencies(app run_preprocessor)

target_include_directories(app PRIVATE ${GENERATED_DIR})
target_include_directories(app PRIVATE include)
target_include_directories(app PRIVATE vendor/include)

target_link_directories(app PRIVATE vendor/lib)


#Relase
#[[
if (WIN32)
    target_link_libraries(app Winter wsock32 ws2_32 pq)
endif (WIN32)
if (UNIX)
    target_link_libraries(app Winter pq)
endif(UNIX)
]]

#Development

file(GLOB_RECURSE LIB_GENERATED_SOURCES "${GENERATED_DIR}/*.cpp" "${LIB_SOURCES_DIR}/*.cpp")
target_sources(app PRIVATE ${LIB_GENERATED_SOURCES} )

if (WIN32)
    target_link_libraries(app wsock32 ws2_32 pq)
endif (WIN32)
if (UNIX)
    target_link_libraries(app pq)
endif(UNIX)

