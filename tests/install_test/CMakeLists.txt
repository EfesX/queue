cmake_minimum_required(VERSION 3.18)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "vcpkg toolchain")

project(install_test)

find_package(equeue CONFIG REQUIRED)

add_executable(install_test install_test.cc)
target_link_libraries(install_test equeue::equeue)
target_include_directories(install_test PRIVATE ${Protobuf_INCLUDE_DIRS})
