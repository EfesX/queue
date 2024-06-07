cmake_minimum_required(VERSION 3.18)
project(fetch_test)

include(FetchContent)

FetchContent_Declare(
  equeue
  GIT_REPOSITORY https://github.com/EfesX/queue.git
  GIT_TAG        9b757473019a1061971db9e645fa87568ce10be3
)
FetchContent_MakeAvailable(equeue)

add_executable(fetch_test fetch_test.cc)
target_link_libraries(fetch_test equeue::equeue)

