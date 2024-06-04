#LibraryInstall(gtest)
find_package(GTest CONFIG REQUIRED)

set(GTEST_LINK GTest::gtest GTest::gtest_main)

add_executable(meta_test meta_test.cc)
target_link_libraries(meta_test ${PROJECT_NAME} ${GTEST_LINK})

add_executable(proto_storage_test proto_storage_test.cc)
target_link_libraries(proto_storage_test ${PROJECT_NAME} ${GTEST_LINK} hw_proto)

add_executable(queue_test queue_test.cc)
target_link_libraries(queue_test ${PROJECT_NAME} ${GTEST_LINK} hw_proto)

add_executable(queue_async_test queue_async_test.cc)
target_link_libraries(queue_async_test ${PROJECT_NAME} ${GTEST_LINK} hw_proto)

enable_testing()
add_test(meta_test meta_test)
add_test(proto_storage_test proto_storage_test)
add_test(queue_test queue_test)
add_test(queue_async_test queue_async_test)

