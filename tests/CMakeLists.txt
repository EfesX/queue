include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(node_test node_test.cc)
target_include_directories(node_test PUBLIC ../include)
target_link_libraries(node_test gtest gtest_main)

enable_testing()
add_test(node_test node_test)
