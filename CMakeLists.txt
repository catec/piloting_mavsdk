cmake_minimum_required(VERSION 3.10.2)

execute_process(
    COMMAND git describe --abbrev=8 --dirty --always --tags
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE VERSION_STR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

STRING(REGEX REPLACE v\([0-9]+\).[0-9]+.[0-9]+.*$ \\1 MAVSDK_VERSION_MAJOR "${VERSION_STR}")
STRING(REGEX REPLACE v[0-9]+.\([0-9]+\).[0-9]+.*$ \\1 MAVSDK_VERSION_MINOR "${VERSION_STR}")

STRING(FIND "${VERSION_STR}" "-" FIND_RESULT)
IF("${FIND_RESULT}" EQUAL -1)
   STRING(REGEX REPLACE v[0-9]+.[0-9]+.\([0-9]+\).*$ \\1 MAVSDK_VERSION_PATCH "${VERSION_STR}")
ELSE()
   STRING(REGEX REPLACE v[0-9]+.[0-9]+.[0-9]+.\([0-9]+\).*$ \\1 MAVSDK_VERSION_PATCH "${VERSION_STR}")
ENDIF()

STRING(CONCAT VERSION_STR "v" ${MAVSDK_VERSION_MAJOR} "." ${MAVSDK_VERSION_MINOR} "." ${MAVSDK_VERSION_PATCH})
message(STATUS "Version: ${VERSION_STR}")
message(STATUS "Version major: ${MAVSDK_VERSION_MAJOR}")
message(STATUS "Version minor: ${MAVSDK_VERSION_MINOR}")
message(STATUS "Version patch: ${MAVSDK_VERSION_PATCH}")

set(MAVSDK_SOVERSION_STRING ${MAVSDK_VERSION_MAJOR})
set(MAVSDK_VERSION_STRING ${MAVSDK_VERSION_MAJOR}.${MAVSDK_VERSION_MINOR}.${MAVSDK_VERSION_PATCH})

project(mavsdk)

option(BUILD_SHARED_LIBS "Build core as shared libraries instead of static ones" ON)

include(GNUInstallDirs)

add_subdirectory(src)

install(EXPORT mavsdk-targets
    FILE MAVSDKTargets.cmake
    NAMESPACE MAVSDK::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MAVSDK
    )

# For the build tree
configure_file(MAVSDKConfig.cmake.in
    "${PROJECT_BINARY_DIR}/MAVSDKConfig.cmake" @ONLY)
# For the install tree
configure_file(MAVSDKConfig.cmake.in
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/MAVSDKConfig.cmake" @ONLY)

install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/MAVSDKConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MAVSDK)