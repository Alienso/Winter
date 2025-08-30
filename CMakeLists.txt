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
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/include/postgreSQL)

link_directories(${PROJECT_SOURCE_DIR}/lib/)
link_directories(${PROJECT_SOURCE_DIR}/lib/postgreSQL)

#temp lib includes
include_directories(${PROJECT_SOURCE_DIR}/source/log)
include_directories(${PROJECT_SOURCE_DIR}/source/reflect)
include_directories(${PROJECT_SOURCE_DIR}/source/serialize)
include_directories(${PROJECT_SOURCE_DIR}/source/util)

set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)

add_custom_command(
        OUTPUT ${GENERATED_DIR}/stamp
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
        COMMAND ${CMAKE_SOURCE_DIR}/cmake-build-debug/PreProcessor.exe
        ${CMAKE_SOURCE_DIR}/source
        ${GENERATED_DIR}
        COMMAND ${CMAKE_COMMAND} -E touch ${GENERATED_DIR}/stamp
        DEPENDS ${CMAKE_SOURCE_DIR}/cmake-build-debug/PreProcessor.exe
        COMMENT "Running PreProcessor to generate sources"
)

# Collect all generated .cpp files after preprocessing
file(GLOB_RECURSE GENERATED_SOURCES
        "${GENERATED_DIR}/*.cpp"
)

add_executable(my_app
        ${GENERATED_SOURCES}
)

add_dependencies(my_app run_preprocessor)

target_include_directories(my_app PRIVATE ${GENERATED_DIR})

if (WIN32)
    target_link_libraries(my_app wsock32 ws2_32 pq)
endif (WIN32)
if (UNIX)
    target_link_libraries(my_app pq)
endif(UNIX)

add_custom_target(run_preprocessor DEPENDS ${GENERATED_DIR}/stamp)