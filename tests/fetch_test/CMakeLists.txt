cmake_minimum_required(VERSION 3.18)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "vcpkg toolchain")

project(fetch_test)

message(STATUS ">>> ${CMAKE_TOOLCHAIN_FILE}")

include(FetchContent)

FetchContent_Declare(
  equeue
  GIT_REPOSITORY https://github.com/EfesX/queue.git
  GIT_TAG        0.1.0
)
FetchContent_MakeAvailable(equeue)

add_executable(fetch_test fetch_test.cc)
target_link_libraries(fetch_test equeue::equeue)

