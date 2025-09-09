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

include_directories(${PROJECT_SOURCE_DIR})
include_directories(include/)
include_directories(vendor/include/)
include_directories(vendor/include/postgreSQL)

link_directories(vendor/lib/)
link_directories(vendor/lib/postgreSQL)

#temp lib includes
include_directories(${PROJECT_SOURCE_DIR}/source/log)
include_directories(${PROJECT_SOURCE_DIR}/source/reflect)
include_directories(${PROJECT_SOURCE_DIR}/source/serialize)
include_directories(${PROJECT_SOURCE_DIR}/source/util)

add_executable(PreProcessor tools/pre_processor/main.cpp
        tools/pre_processor/PreProcessor.cpp tools/pre_processor/PreProcessor.h
        tools/pre_processor/pass/ReflectionPass.cpp tools/pre_processor/pass/ReflectionPass.h
        tools/pre_processor/pass/Pass.cpp tools/pre_processor/pass/Pass.h
        tools/pre_processor/pass/AnnotationPass.cpp tools/pre_processor/pass/AnnotationPass.h
        tools/pre_processor/pass/ComponentPass.cpp tools/pre_processor/pass/ComponentPass.h
        tools/pre_processor/util.h)


set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)

add_custom_command(
        OUTPUT ${GENERATED_DIR}/stamp
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
        COMMAND $<TARGET_FILE:PreProcessor> ${CMAKE_SOURCE_DIR}/source ${GENERATED_DIR}
        COMMAND ${CMAKE_COMMAND} -E touch ${GENERATED_DIR}/stamp
        DEPENDS PreProcessor
        COMMENT "Running PreProcessor to generate sources"
)

add_custom_target(run_preprocessor
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
        COMMAND PreProcessor ${CMAKE_SOURCE_DIR}/source ${GENERATED_DIR}
        DEPENDS PreProcessor
        COMMENT "Running PreProcessor..."
)

file(WRITE ${CMAKE_BINARY_DIR}/dummy.cpp "// placeholder")
add_executable(Winter ${CMAKE_BINARY_DIR}/dummy.cpp)

# Collect all generated .cpp files after preprocessing
file(GLOB_RECURSE GENERATED_SOURCES "${GENERATED_DIR}/*.cpp")
target_sources(Winter PRIVATE ${GENERATED_SOURCES})
add_dependencies(Winter run_preprocessor)
target_include_directories(Winter PRIVATE ${GENERATED_DIR})

if (WIN32)
    target_link_libraries(Winter wsock32 ws2_32 pq)
endif (WIN32)
if (UNIX)
    target_link_libraries(Winter pq)
endif(UNIX)