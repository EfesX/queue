
if(CPPCHECK_ENABLE)
    find_program(cppcheck_exe NAMES cppcheck)

    if(NOT cppcheck_exe)
        message(WARNING "cppcheck is not found. linting will be skipped")
    else()
        message(STATUS "Found cppcheck: ${cppcheck_exe}")
        set(cppcheck_opts --enable=all --inline-suppr --quiet --suppressions-list=${PROJECT_SOURCE_DIR}/cppcheck.supp --suppress=unmatchedSuppression)
        set(CMAKE_C_CPPCHECK ${cppcheck_exe} --std=c11 ${cppcheck_opts})
        set(CMAKE_CXX_CPPCHECK ${cppcheck_exe} --std=c++20 ${cppcheck_opts})
    endif()
endif()


if(CLANGTIDY_ENABLE)
    find_program(clangtidy_exe NAMES clang-tidy)
    if(NOT clangtidy_exe)
        message(WARNING "clang-tidy is not found. linting will be skipped")
    else()
        message(STATUS "Found clang-tidy: ${clangtidy_exe}")
        #set(tidy_opts --header-filter=^${PROJECT_SOURCE_DIR}/,-${PROJECT_SOURCE_DIR}/build/)
        set(CMAKE_C_CLANG_TIDY ${clangtidy_exe} --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy)
        set(CMAKE_CXX_CLANG_TIDY ${clangtidy_exe} --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy)
    endif()

endif()