

find_package(Protobuf CONFIG)

if(NOT ${Protobuf_FOUND})
    set(FETCHCONTENT_QUIET OFF)
    set(ABSL_PROPAGATE_CXX_STD ON)
    set(ABSL_ENABLE_INSTALL ON)

    include(FetchContent)

    FetchContent_Declare(
            protobuf
            GIT_REPOSITORY https://github.com/google/protobuf.git
            GIT_TAG        v3.19.3
            GIT_PROGRESS   TRUE
            GIT_SHALLOW    TRUE
            USES_TERMINAL_DOWNLOAD TRUE
            GIT_SUBMODULES_RECURSE FALSE
            GIT_SUBMODULES ""
            SOURCE_SUBDIR  cmake
            OVERRIDE_FIND_PACKAGE
    )
    set(protobuf_BUILD_TESTS OFF CACHE INTERNAL "")

    FetchContent_GetProperties(protobuf)
    FetchContent_MakeAvailable(protobuf)

    if(NOT TARGET protoc)
        message(FATAL_ERROR "Can not find target protoc")
    endif()
    
    find_package(Protobuf CONFIG REQUIRED)
    set(PROTOC_EXEC "${CMAKE_CURRENT_BINARY_DIR}/../_deps/protobuf-build/protoc")
else()
    find_program(PROTOC_EXEC NAMES protoc REQUIRED)
    message(STATUS "Using protoc ${PROTOC_EXEC}")
endif()


########################################################################
#find_package(gRPC CONFIG REQUIRED)

#find_program(GRPC_PLUGIN NAMES "grpc_cpp_plugin")
#message(STATUS "Found grpc_cpp_plugin : " ${GRPC_PLUGIN})

########################################################################

set(HW_PROTO hw_proto)

get_filename_component(${HW_PROTO} "${CMAKE_SOURCE_DIR}/proto/queue.proto" ABSOLUTE)
get_filename_component(hw_proto_path ${hw_proto} PATH)

set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/queue.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/queue.pb.h")

add_custom_command(
    OUTPUT ${hw_proto_srcs} ${hw_proto_hdrs}
    COMMAND ${PROTOC_EXEC}
    ARGS --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
#        --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
        -I ${hw_proto_path}
        --plugin=protoc-gen-grpc=${GRPC_PLUGIN}
        ${hw_proto}
    DEPENDS ${hw_proto} protoc
)

add_library(${HW_PROTO} ${hw_proto_srcs} ${hw_proto_hdrs})
target_link_libraries(${HW_PROTO} PUBLIC protobuf::libprotobuf)
target_include_directories(${HW_PROTO} INTERFACE ${CMAKE_CURRENT_BINARY_DIR})