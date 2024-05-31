find_package(GTest CONFIG)
if(NOT ${GTest_FOUND})
  message(STATUS "Downloading GTest...")
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_PROGRESS   TRUE
    GIT_SHALLOW    TRUE
    USES_TERMINAL_DOWNLOAD TRUE
  )
  FetchContent_MakeAvailable(googletest)
  include(GoogleTest)
  set(GTEST_LINK gtest gtest_main)
else()
  set(GTEST_LINK GTest::gtest GTest::gtest_main)
endif()


add_executable(queue_test queue_test.cc)
target_link_libraries(queue_test ${PROJECT_NAME} ${GTEST_LINK} hw_proto)

add_executable(meta_test meta_test.cc)
target_link_libraries(meta_test ${PROJECT_NAME} ${GTEST_LINK})

add_executable(proto_storage_test proto_storage_test.cc)
target_link_libraries(proto_storage_test ${PROJECT_NAME} ${GTEST_LINK} hw_proto)

enable_testing()
add_test(meta_test meta_test)
add_test(queue_test queue_test)
add_test(proto_storage_test proto_storage_test)
