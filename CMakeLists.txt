cmake_minimum_required(VERSION 3.20)
project(equeue)

option(BUILD_TESTS "Build the unit tests" ON)

set(CMAKE_CXX_STANDARD 20)

if(${BUILD_TESTS})
    add_subdirectory(tests)
endif()