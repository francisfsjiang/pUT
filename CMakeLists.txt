cmake_minimum_required(VERSION 3.3)
project(put)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories($ENV{BOOST_ROOT})

link_directories($ENV{BOOST_ROOT}/stage/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++1y")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D PUT_DEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")

set(COMMON_SRC_FILES
        src/inet_address.cpp
        )

set(CLIENT_SRC_FILES
        src/client/client.cpp
        src/client/client_process.cpp
        ${COMMON_SRC_FILES})

set(SERVER_SRC_FILES
        src/server/server.cpp
        ${COMMON_SRC_FILES})

add_executable(client ${CLIENT_SRC_FILES})
add_executable(server ${SERVER_SRC_FILES})

target_link_libraries(client boost_program_options)

