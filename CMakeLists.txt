cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "Build the unit tests" ON)

project(equeue LANGUAGES C CXX)

file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_SOURCE_DIR}/include/*.hpp)

add_custom_target(
    lint
    COMMAND clang-tidy
    ${ALL_SOURCE_FILES}
    --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
)

add_subdirectory(proto)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
    FILE_SET HEADERS
    BASE_DIRS "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}"
    FILES
        detail/meta.hpp
        detail/proto_storage.hpp
        detail/types.hpp
        detail/utils.hpp
        queue.hpp
        proto/queue.pb.h
        proto/queue.grpc.pb.h
)

if(${BUILD_TESTS})
    add_subdirectory(tests)
endif()
