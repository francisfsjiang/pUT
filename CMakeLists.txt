cmake_minimum_required(VERSION 3.3)
project(put)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories($ENV{BOOST_ROOT})

link_directories($ENV{BOOST_ROOT}/stage/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++1y")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D DC_DEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")

set(SRC_FILES src/put_socket.cpp)

add_executable(server src/server.cpp)
add_executable(client src/client.cpp)
target_link_libraries(client boost_program_options)

