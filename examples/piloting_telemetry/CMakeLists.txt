cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(piloting_telemetry)

if(NOT MSVC)
    add_definitions("-Wall -Wextra")
else()
    add_definitions("-WX -W2")
endif()

find_package(MAVSDK REQUIRED)

add_executable(piloting_gcs_telemetry_example
    piloting_gcs_telemetry_example.cpp
)

target_link_libraries(piloting_gcs_telemetry_example
    MAVSDK::mavsdk
    MAVSDK::mavsdk_telemetry
)

add_executable(piloting_robot_telemetry_example
    piloting_robot_telemetry_example.cpp
)

target_link_libraries(piloting_robot_telemetry_example
    MAVSDK::mavsdk
    MAVSDK::mavsdk_telemetry_robotic_vehicle
)
