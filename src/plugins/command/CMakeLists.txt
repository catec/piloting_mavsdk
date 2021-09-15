add_library(mavsdk_command
    command.cpp
    command_impl.cpp
)

target_link_libraries(mavsdk_command
    mavsdk
)

set_target_properties(mavsdk_command
    PROPERTIES COMPILE_FLAGS "${warnings}"
    VERSION "${MAVSDK_VERSION_STRING}"
    SOVERSION "${MAVSDK_SOVERSION_STRING}"
)

target_include_directories(mavsdk_command PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/mavsdk>
    )

install(TARGETS mavsdk_command
    EXPORT mavsdk-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(FILES
    include/plugins/command/command.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mavsdk/plugins/command
)
