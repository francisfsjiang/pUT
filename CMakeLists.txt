cmake_minimum_required(VERSION 3.2)
project(put)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++1y")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D PUT_DEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")

# make __filename__ marco
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/src/,,$(abspath $<))\"'")

set(COMMON_SRC_FILES
        src/inet_address.cpp
        src/log/logger.cpp
        src/log/log_stream.cpp
        src/log/defalut_log_output.cpp
        )

set(CLIENT_SRC_FILES
        src/client/client.cpp
        src/client/client_process.cpp
        src/client/client_cfg.cpp
        ${COMMON_SRC_FILES})

set(SERVER_SRC_FILES
        src/server/server.cpp
        ${COMMON_SRC_FILES})

add_executable(client ${CLIENT_SRC_FILES})
add_executable(server ${SERVER_SRC_FILES})

target_link_libraries(client boost_program_options.a boost_filesystem.a)

