add_library(mavsdk_command_robotic_vehicle
    command_robotic_vehicle.cpp
    command_robotic_vehicle_impl.cpp
)

target_link_libraries(mavsdk_command_robotic_vehicle
    mavsdk
)

set_target_properties(mavsdk_command_robotic_vehicle
    PROPERTIES COMPILE_FLAGS ${warnings}
    VERSION ${MAVSDK_VERSION_STRING}
    SOVERSION ${MAVSDK_SOVERSION_STRING}
)

target_include_directories(mavsdk_command_robotic_vehicle PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/mavsdk>
    )

install(TARGETS mavsdk_command_robotic_vehicle
    EXPORT mavsdk-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(FILES
    include/plugins/command_robotic_vehicle/command_robotic_vehicle.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mavsdk/plugins/command_robotic_vehicle
)
