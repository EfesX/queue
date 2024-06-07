cmake_minimum_required(VERSION 3.14)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "vcpkg toolchain")

include(cmake/prelude.cmake)

project(
    equeue
    VERSION 0.1.0
    DESCRIPTION "A universal queue with thread-safe features that can operate over gRPC"
    HOMEPAGE_URL "https://github.com/EfesX/queue"
    LANGUAGES CXX
)

include(cmake/variables.cmake)
include(cmake/linting.cmake)

# ---- Generate proto files
include(proto/equeue-proto.cmake)

# ---- Declare library ----
# hw_*_srcs - list of generated files from equeue-proto.cmake
add_library(equeue ${hw_proto_srcs} ${hw_grpc_srcs})
add_library(equeue::equeue ALIAS equeue)

# Зачем вот это???
include(GenerateExportHeader)
generate_export_header(
    equeue
    BASE_NAME equeue
    EXPORT_FILE_NAME export/equeue/equeue_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

set_target_properties(
    equeue PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME equeue
    OUTPUT_NAME equeue
)

target_include_directories(
    equeue 
    PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
        "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>"
        "\$<BUILD_INTERFACE:${Protobuf_INCLUDE_DIRS}>"
        "\$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(
    equeue SYSTEM PUBLIC
        "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(equeue PUBLIC cxx_std_20)

# queue_proto_libraries - list of libraries from equeue-proto.cmake
target_link_libraries(equeue PRIVATE ${queue_proto_libraries})

# ---- Install rules ----
if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Tests ----
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# ---- Examples ----
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()