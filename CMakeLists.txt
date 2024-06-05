cmake_minimum_required(VERSION 3.28)

#set(VCPKG_ROOT "vcpkg" CACHE PATH "vcpkg root dir")
#set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "toolchain")

set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "Build the unit tests" ON)

project(equeue LANGUAGES C CXX)

file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_SOURCE_DIR}/*.hpp)

add_custom_target(
    lint
    COMMAND clang-tidy
    ${ALL_SOURCE_FILES}
    --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
)

#if(UNIX)
#    if(NOT EXISTS ${VCPKG_ROOT}/vcpkg) 
#        execute_process(COMMAND ./bootstrap-vcpkg.sh -disableMetrics WORKING_DIRECTORY ${VCPKG_ROOT})
#    endif()
#    set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg CACHE FILEPATH "vcpkg executable")
#elseif(WIN32)
#    if(NOT EXISTS ${VCPKG_ROOT}/vcpkg) 
#        execute_process(COMMAND ./bootstrap-vcpkg.bat -disableMetrics WORKING_DIRECTORY ${VCPKG_ROOT})
#    endif()
#    set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg.exe CACHE FILEPATH "vcpkg executable")
#else()
#    message(FATAL_ERROR "Not Recognized OS Error")
#endif()
#
#function(LibraryInstall LIB)
#    execute_process(COMMAND ${VCPKG_EXEC} install ${LIB})
#endfunction()

#############################################################
add_subdirectory(proto)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
    FILE_SET HEADERS
    BASE_DIRS "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}"
    FILES
        detail/meta.hpp
        detail/proto_storage.hpp
        detail/types.hpp
        detail/utils.hpp
        queue.hpp
        proto/queue.pb.h
        proto/queue.grpc.pb.h
)

if(${BUILD_TESTS})
    add_subdirectory(tests)
endif()
