cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0025 NEW)

project(mavsdk)

# ccache
option(CCACHE "Use ccache if available" ON)
find_program(CCACHE_PROGRAM ccache)
if(CCACHE AND CCACHE_PROGRAM AND NOT DEFINED ENV{CCACHE_DISABLE})

    get_filename_component(ccache_real_path ${CCACHE_PROGRAM} REALPATH)
    get_filename_component(cxx_real_path ${CMAKE_CXX_COMPILER} REALPATH)
    get_filename_component(cxx_abs_path ${CMAKE_CXX_COMPILER} ABSOLUTE)

    if ("${ccache_real_path}" STREQUAL "${cxx_real_path}")
        message(STATUS "ccache enabled via symlink (${cxx_abs_path} -> ${cxx_real_path})")
    else()
        message(STATUS "ccache enabled (export CCACHE_DISABLE=1 to disable)")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    endif()
endif()

option(CMAKE_POSITION_INDEPENDENT_CODE "Position independent code" ON)

include(cmake/compiler_flags.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

find_package(Threads REQUIRED)

if(ANDROID)
    set(lib_path "lib/android/${ANDROID_ABI}")
elseif(IOS)
    set(lib_path "lib/ios")
else()
    set(lib_path "lib")
endif()

set(mavsdk_install_include_dir "include/mavsdk")
set(mavsdk_install_lib_dir ${lib_path})

add_subdirectory(core)
add_subdirectory(plugins)
add_subdirectory(mavlink/include/mavlink/v2.0)

if (DEFINED EXTERNAL_DIR AND NOT EXTERNAL_DIR STREQUAL "")
    add_subdirectory(${EXTERNAL_DIR}/plugins
        ${CMAKE_CURRENT_BINARY_DIR}/${EXTERNAL_DIR}/plugins)
    include_directories(${EXTERNAL_DIR})
endif()