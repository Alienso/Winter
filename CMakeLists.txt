cmake_minimum_required(VERSION 3.16)
project(Winter)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter -g -O0")

include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/include/postgreSQL)

link_directories(${PROJECT_SOURCE_DIR}/lib/)
link_directories(${PROJECT_SOURCE_DIR}/lib/postgreSQL)

#temp lib includes
include_directories(${PROJECT_SOURCE_DIR}/source/log)
include_directories(${PROJECT_SOURCE_DIR}/source/reflect)
include_directories(${PROJECT_SOURCE_DIR}/source/serialize)
include_directories(${PROJECT_SOURCE_DIR}/source/util)

#pre-process app dir and use it for compilation

set(SOURCES source/Winter.cpp source/Winter.h
        source/http/HttpServer.cpp source/http/HttpServer.h
        source/http/HttpRequest.cpp source/http/HttpRequest.h
        source/http/URI.cpp source/http/URI.h
        source/log/Logger.cpp source/log/Logger.h
        source/log/Appender.cpp source/log/Appender.h
        source/Configuration.cpp source/Configuration.h
        source/util/tsqueue.h source/util/stringUtils.h
        source/http/Connection.cpp source/http/Connection.h
        source/http/HttpResponse.cpp source/http/HttpResponse.h
        source/http/httpConstants.h source/http/httpConstants.cpp
        source/Router.cpp source/Router.h
        source/app/controller/MyController.cpp source/app/controller/MyController.h
        source/app/service/MyService.cpp source/app/service/MyService.h
        source/reflect/Reflect.cpp source/reflect/Reflect.h
        source/reflect/Field.cpp source/reflect/Field.h
        source/reflect/Method.cpp source/reflect/Method.h
        source/serialize/JsonDeserializer.cpp source/serialize/JsonDeserializer.h
        source/serialize/JsonSerializer.cpp source/serialize/JsonSerializer.h
        source/serialize/FieldTypeEnums.h source/http/ControllerMapping.h
        source/util/ThreadPool.h source/util/util.h
        source/Component.cpp source/Component.h
        source/app/dto/BaseRequest.h source/app/dto/BaseResponse.h source/app/dto/InnerClass.h source/sql/Connection.h source/sql/Statement.h source/sql/postgres/PgConnection.cpp source/sql/postgres/PgConnection.h source/sql/postgres/PgStatement.cpp source/sql/postgres/PgStatement.h source/sql/postgres/PgPreparedStatement.cpp source/sql/postgres/PgPreparedStatement.h source/sql/ResultSet.h source/sql/postgres/PgResultSet.cpp source/sql/postgres/PgResultSet.h)

#add_executable(Winter ${SOURCES})

#target_link_libraries(Winter wsock32 ws2_32)

#add_custom_command(TARGET Winter PRE_BUILD
 #       COMMAND ${CMAKE_COMMAND} -E copy_directory
  #      ${PROJECT_SOURCE_DIR}/source/app $<TARGET_FILE_DIR:Winter>/app)

#---------------------------------------------
#Target Build

list(TRANSFORM SOURCES REPLACE source/ target/)

set(targetLen 0)

list(LENGTH SOURCES sourceLen )
message(SOURCES LEN=${sourceLen})

foreach(X IN LISTS SOURCES)
    if (EXISTS ${PROJECT_SOURCE_DIR}/${X})
        MATH(EXPR targetLen "${targetLen}+1")
    endif()
endforeach()

message(TARGET LEN=${targetLen})

if (${sourceLen} STREQUAL  ${targetLen})

    add_executable(Target target/main.cpp ${SOURCES} )

    if (WIN32)
        target_link_libraries(Target wsock32 ws2_32)
    endif (WIN32)

    add_custom_command(TARGET Target PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/source/app $<TARGET_FILE_DIR:Target>/app)

endif()

#list(TRANSFORM SOURCES PREPEND target/)
#if (EXISTS ${PROJECT_SOURCE_DIR}/target/main.cpp)

#set(SOURCES target/main.cpp
#        target/TestClass.cpp target/TestClass.h
#        source/reflect/Reflect.cpp source/reflect/Reflect.h
#        source/reflect/Field.cpp source/reflect/Field.h
#        source/reflect/Method.cpp source/reflect/Method.h
#        source/serialize/JsonDeserializer.cpp source/serialize/JsonDeserializer.h
#        source/serialize/FieldEnums.h
#        source/serialize/JsonSerializer.cpp source/serialize/JsonSerializer.h
#        source/util/ThreadPool.h)
#endif()

#---------------------------------------------
#PreProcessor

add_executable(PreProcessor pre_processor/main.cpp
        pre_processor/PreProcessor.cpp pre_processor/PreProcessor.h
        pre_processor/pass/ReflectionPass.cpp pre_processor/pass/ReflectionPass.h
        pre_processor/pass/Pass.cpp pre_processor/pass/Pass.h
        pre_processor/pass/AnnotationPass.cpp pre_processor/pass/AnnotationPass.h pre_processor/pass/ComponentPass.cpp pre_processor/pass/ComponentPass.h)


#---------------------------------------------
#Tests
add_executable(Tests ${SOURCES}
        tests/runTests.cpp)
target_link_libraries(Tests wsock32 ws2_32)


#---------------------------------------
#Temp
#add_executable(Temp ./temp/main.cpp)
#target_link_libraries(Temp)