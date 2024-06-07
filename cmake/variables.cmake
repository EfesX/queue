string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}" PROJECT_IS_TOP_LEVEL)

option(BUILD_EXAMPLES   "Build examples"  OFF)
option(CPPCHECK_ENABLE  "Static linting with cppcheck" OFF)
option(CLANGTIDY_ENABLE "Static linting width clang-tidy" OFF)

message(STATUS "===============================================")
message(STATUS "Project using following variables:")
message(STATUS "PROJECT_IS_TOP_LEVEL  : ${PROJECT_IS_TOP_LEVEL}")
message(STATUS "BUILD_EXAMPLES        : ${BUILD_EXAMPLES}")
message(STATUS "CMAKE_TOOLCHAIN_FILE  : ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CPPCHECK_ENABLE       : ${CPPCHECK_ENABLE}")
message(STATUS "CLANGTIDY_ENABLE      : ${CLANGTIDY_ENABLE}")
message(STATUS "===============================================")
