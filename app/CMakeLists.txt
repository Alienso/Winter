cmake_minimum_required(VERSION 3.16)
project(App)

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


#[[
add_executable(app main.cpp
        source/controller/MyController.cpp source/controller/MyController.h
        source/dto/AllFieldsDTO.h source/dto/AllFieldsVecDTO.h source/dto/BaseRequest.h source/dto/BaseResponse.h
        source/entity/PlayerEntity.h
        source/repository/PlayerRepository.cpp source/repository/PlayerRepository.h
        source/service/MyService.cpp source/service/MyService.h
)


target_link_libraries(app PRIVATE Winter)
target_include_directories(app PRIVATE ../include)
target_include_directories(app PRIVATE ../vendor/include)

if (WIN32)
    target_link_libraries(Winter wsock32 ws2_32 pq)
endif (WIN32)
if (UNIX)
    target_link_libraries(Winter pq)
endif(UNIX)

]]