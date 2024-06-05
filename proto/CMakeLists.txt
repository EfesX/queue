
#LibraryInstall(protobuf)
#LibraryInstall(grpc)

########################################################################
include(FindProtobuf)
find_package(gRPC CONFIG REQUIRED)

find_program(PROTOC_EXE NAMES "protoc-${Protobuf_VERSION}.0" "protoc-${Protobuf_VERSION}.0.exe" REQUIRED)
find_program(GRPC_PLUGIN NAMES "grpc_cpp_plugin" "grpc_cpp_plugin.exe" REQUIRED)

message(STATUS "Found protoc : " ${PROTOC_EXE})
message(STATUS "Found grpc_cpp_plugin : " ${GRPC_PLUGIN})
########################################################################

set(HW_PROTO hw_proto)

get_filename_component(${HW_PROTO} "${CMAKE_SOURCE_DIR}/proto/queue.proto" ABSOLUTE)
get_filename_component(hw_proto_path ${hw_proto} PATH)

set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/queue.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/queue.pb.h")

set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/queue.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/queue.grpc.pb.h")

add_custom_command(
    OUTPUT ${hw_proto_srcs} ${hw_proto_hdrs} ${hw_grpc_srcs} ${hw_grpc_hdrs}
    COMMAND ${PROTOC_EXE}
    ARGS --cpp_out ${CMAKE_CURRENT_BINARY_DIR}
        --grpc_out ${CMAKE_CURRENT_BINARY_DIR}
        -I ${hw_proto_path}
        --plugin=protoc-gen-grpc=${GRPC_PLUGIN}
        ${hw_proto}
    DEPENDS ${hw_proto}
)

add_library(${HW_PROTO} ${hw_proto_srcs} ${hw_proto_hdrs} ${hw_grpc_srcs} ${hw_grpc_hdrs})
target_link_libraries(${HW_PROTO} PUBLIC ${Protobuf_LIBRARY} gRPC::grpc gRPC::grpc++)
target_include_directories(${HW_PROTO} INTERFACE ${CMAKE_CURRENT_BINARY_DIR})