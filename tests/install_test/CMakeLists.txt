cmake_minimum_required(VERSION 3.18)
project(install_test)

find_package(equeue CONFIG REQUIRED)

add_executable(install_test install_test.cc)
target_link_libraries(install_test equeue::equeue)
target_include_directories(install_test PRIVATE ${Protobuf_INCLUDE_DIRS})
