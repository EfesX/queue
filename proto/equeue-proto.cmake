
########################################################################
include(FindProtobuf)
find_package(gRPC CONFIG REQUIRED)

find_program(PROTOC_EXE NAMES "protoc-${Protobuf_VERSION}.0" "protoc-${Protobuf_VERSION}.0.exe" REQUIRED)
find_program(GRPC_PLUGIN NAMES "grpc_cpp_plugin" "grpc_cpp_plugin.exe" REQUIRED)

message(STATUS "Found protoc : " ${PROTOC_EXE})
message(STATUS "Found grpc_cpp_plugin : " ${GRPC_PLUGIN})
########################################################################

set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/proto/queue.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/proto/queue.pb.h")

set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/proto/queue.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/proto/queue.grpc.pb.h")

set_source_files_properties(${hw_proto_srcs} PROPERTIES SKIP_LINTING ON)
set_source_files_properties(${hw_proto_hdrs} PROPERTIES SKIP_LINTING ON)
set_source_files_properties(${hw_grpc_srcs} PROPERTIES SKIP_LINTING ON)
set_source_files_properties(${hw_grpc_hdrs} PROPERTIES SKIP_LINTING ON)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/proto)
file(COPY "${CMAKE_CURRENT_LIST_DIR}/../proto/queue.proto" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/proto)

add_custom_command(
    OUTPUT ${hw_proto_srcs} ${hw_proto_hdrs} ${hw_grpc_srcs} ${hw_grpc_hdrs}
    COMMAND ${PROTOC_EXE}
    ARGS --cpp_out "proto"
        --grpc_out "proto"
        -I "${CMAKE_CURRENT_LIST_DIR}/../proto"
        --plugin=protoc-gen-grpc=${GRPC_PLUGIN}
        "${CMAKE_CURRENT_LIST_DIR}/../proto/queue.proto"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set(queue_proto_libraries ${Protobuf_LIBRARY} gRPC::grpc gRPC::grpc++)
