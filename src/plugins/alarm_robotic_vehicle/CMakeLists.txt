add_library(mavsdk_alarm_robotic_vehicle
    alarm_robotic_vehicle.cpp
    alarm_robotic_vehicle_impl.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

set_target_properties(mavsdk_alarm_robotic_vehicle
    PROPERTIES COMPILE_FLAGS ${warnings}
    VERSION "${MAVSDK_VERSION_STRING}"
    SOVERSION "${MAVSDK_SOVERSION_STRING}"
)

target_link_libraries(mavsdk_alarm_robotic_vehicle
    PUBLIC
    mavsdk
)

target_include_directories(mavsdk_alarm_robotic_vehicle PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/mavsdk>
    )

install(TARGETS mavsdk_alarm_robotic_vehicle
    EXPORT mavsdk-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(FILES
    include/plugins/alarm_robotic_vehicle/alarm_robotic_vehicle.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mavsdk/plugins/alarm_robotic_vehicle
)
