cmake_minimum_required(VERSION 3.10.2)
project(mavsdk_examples)

# This allows to build all examples at once.

if (WERROR)
    add_compile_options(-Werror)
endif()

add_subdirectory(piloting_inspection)
add_subdirectory(piloting_telemetry)
add_subdirectory(piloting_command)
add_subdirectory(piloting_checklist)
add_subdirectory(piloting_alarm)

