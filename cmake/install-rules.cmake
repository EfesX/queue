if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_INSTALL_INCLUDEDIR "include/equeue-${PROJECT_VERSION}" CACHE STRING "")
    set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package equeue)

install(
    DIRECTORY "include/equeue" "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT equeue-dev
)
install(
    FILES 
        "${PROJECT_BINARY_DIR}/proto/queue.pb.h"
        "${PROJECT_BINARY_DIR}/proto/queue.grpc.pb.h"
        "${PROJECT_BINARY_DIR}/proto/queue.proto"
    DESTINATION 
        "${CMAKE_INSTALL_INCLUDEDIR}/proto" 
    COMPONENT equeue-dev
)

install(
    TARGETS equeue
    EXPORT equeueTargets
    RUNTIME #
    COMPONENT equeue-run
    LIBRARY #
    COMPONENT equeue-run
    NAMELINK_COMPONENT equeue-dev
    ARCHIVE #
    COMPONENT equeue-dev
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    equeue_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE equeue_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(equeue_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${equeue_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT equeue-dev
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${equeue_INSTALL_CMAKEDIR}"
    COMPONENT equeue-dev
)

install(
    EXPORT equeueTargets
    NAMESPACE equeue::
    DESTINATION "${equeue_INSTALL_CMAKEDIR}"
    COMPONENT equeue-dev
)

if(PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif()
