cmake_minimum_required(VERSION 3.20)
project(equeue)

option(BUILD_TESTS "Build the unit tests" ON)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(proto)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
    FILE_SET HEADERS
    BASE_DIRS "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}"
    FILES
        detail/list_storage.hpp
        detail/meta.hpp
        detail/node.hpp
        detail/proto_storage.hpp
        detail/types.hpp
        detail/utils.hpp
        queue.hpp
        proto/queue.pb.h
)


if(${BUILD_TESTS})
    add_subdirectory(tests)
endif()
