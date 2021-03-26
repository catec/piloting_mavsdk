set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(
    SYSTEM ${PROJECT_SOURCE_DIR}/mavlink/include
)

configure_file(version.h.in version.h)

add_library(mavsdk
    call_every_handler.cpp
    connection.cpp
    connection_result.cpp
    system.cpp
    system_impl.cpp
    mavsdk.cpp
    mavsdk_impl.cpp
    global_include.cpp
    mavlink_channels.cpp
    mavlink_alarm_transfer.cpp
    mavlink_alarm_transfer_robotic_vehicle.cpp
    mavlink_alarm_transfer_ground_station.cpp
    mavlink_checklist_transfer.cpp
    mavlink_checklist_transfer_robotic_vehicle.cpp
    mavlink_checklist_transfer_ground_station.cpp
    mavlink_hl_action_transfer.cpp
    mavlink_hl_action_transfer_robotic_vehicle.cpp
    mavlink_hl_action_transfer_ground_station.cpp
    mavlink_inspection_transfer.cpp
    mavlink_inspection_transfer_robotic_vehicle.cpp
    mavlink_inspection_transfer_ground_station.cpp
    mavlink_command_transfer.cpp
    mavlink_command_transfer_robotic_vehicle.cpp
    mavlink_command_transfer_ground_station.cpp
    mavlink_receiver.cpp
    mavlink_message_handler.cpp
    ping.cpp
    alarm_base.cpp
    hl_action_base.cpp
    checklist_base.cpp
    inspection_base.cpp
    telemetry_base.cpp
    command_base.cpp
    plugin_impl_base.cpp
    timeout_handler.cpp
    udp_connection.cpp
    log.cpp
    cli_arg.cpp
    geometry.cpp
    timesync.cpp
)

target_link_libraries(mavsdk
    PRIVATE
    Threads::Threads
)
set_target_properties(mavsdk PROPERTIES
    VERSION ${MAVSDK_VERSION_STRING}
    SOVERSION ${MAVSDK_SOVERSION_STRING}
)

if (IOS)
    target_link_libraries(mavsdk
        PUBLIC
        "-framework Foundation"
        "-framework Security"
    )
endif()

# Link to Windows networking lib.
if (MSVC OR MINGW)
    target_link_libraries(mavsdk
        PRIVATE
        ws2_32
    )
endif()

if(ANDROID)
    target_link_libraries(mavsdk
        PRIVATE
        log
    )
endif()

if( (BUILD_STATIC_MAVSDK_SERVER AND ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")) 
    OR (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "(armv6|armv7)") )
    target_link_libraries(mavsdk PRIVATE atomic)
endif()

set_target_properties(mavsdk
    PROPERTIES COMPILE_FLAGS ${warnings}
    )

target_include_directories(mavsdk
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

install(TARGETS mavsdk
    EXPORT mavsdk-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(FILES
    connection_result.h
    deprecated.h
    system.h
    mavsdk.h
    plugin_base.h
    alarm_base.h
    hl_action_base.h
    checklist_base.h
    inspection_base.h
    telemetry_base.h
    command_base.h
    geometry.h
    mavlink_include.h
    locked_queue.h
    global_include.h
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mavsdk"
)
