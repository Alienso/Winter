cmake_minimum_required(VERSION 3.16)
project(ExampleProject)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(lib)
add_subdirectory(tools/pre_processor)
add_subdirectory(app)


set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)

add_custom_command(
        OUTPUT ${GENERATED_DIR}/stamp
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
        COMMAND $<TARGET_FILE:PreProcessor> ${CMAKE_SOURCE_DIR}/app/source ${GENERATED_DIR}
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
add_executable(app ${CMAKE_BINARY_DIR}/dummy.cpp)

# Collect all generated .cpp files after preprocessing
file(GLOB_RECURSE GENERATED_SOURCES "${GENERATED_DIR}/*.cpp")
target_sources(app PRIVATE ${GENERATED_SOURCES})
add_dependencies(app run_preprocessor)
target_include_directories(app PRIVATE ${GENERATED_DIR})