cmake_minimum_required(VERSION 3.20)
project(equeue)

option(BUILD_TESTS "Build the unit tests" ON)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(proto)

file(COPY "${CMAKE_CURRENT_BINARY_DIR}/proto/queue.pb.h" DESTINATION "${CMAKE_SOURCE_DIR}/include/proto")

if(${BUILD_TESTS})
    add_subdirectory(tests)
endif()
