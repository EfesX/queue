cmake_minimum_required(VERSION 3.14)
project(queue_tests LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}" PROJECT_IS_TOP_LEVEL)

# ---- Dependencies ----
if(PROJECT_IS_TOP_LEVEL)
    find_package(equeue REQUIRED)
endif()

find_package(GTest CONFIG REQUIRED)
enable_testing()

set(GTEST_LINK GTest::gtest GTest::gtest_main)

#---- Tests ----
function(AddTest NAME_TEST SRC)
    add_executable(${NAME_TEST} ${SRC})
    target_link_libraries(${NAME_TEST} equeue::equeue ${GTEST_LINK})
    add_test(${NAME_TEST} ${NAME_TEST})
    #set_source_files_properties(${SRC} PROPERTIES SKIP_LINTING ON)
endfunction()

AddTest(meta_test meta_test.cc)
AddTest(proto_storage_test proto_storage_test.cc)
AddTest(queue_test queue_test.cc)
AddTest(ts_queue_test ts_queue_test.cc)
AddTest(queue_web_test queue_web_test.cc)
