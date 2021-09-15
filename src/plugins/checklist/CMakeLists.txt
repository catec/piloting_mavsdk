add_library(mavsdk_checklist
    checklist.cpp
    checklist_impl.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

set_target_properties(mavsdk_checklist
    PROPERTIES COMPILE_FLAGS ${warnings}
    VERSION "${MAVSDK_VERSION_STRING}"
    SOVERSION "${MAVSDK_SOVERSION_STRING}"
)

target_link_libraries(mavsdk_checklist
    PUBLIC
    mavsdk
)

target_include_directories(mavsdk_checklist PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/mavsdk>
    )

install(TARGETS mavsdk_checklist
    EXPORT mavsdk-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

install(FILES
    include/plugins/checklist/checklist.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mavsdk/plugins/checklist
)
