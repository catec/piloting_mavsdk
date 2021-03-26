# General

file(GLOB MAVLINK_HEADERS
   *.h
)

message("MAVLINK_HEADERS are:")
message("${MAVLINK_HEADERS}")

install(FILES
    ${MAVLINK_HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mavsdk/mavlink/v2.0"
)

# Common

file(GLOB MAVLINK_HEADERS_COMMON
   common/*.h
)

message("MAVLINK_HEADERS_COMMON are:")
message("${MAVLINK_HEADERS_COMMON}")

install(FILES
    ${MAVLINK_HEADERS_COMMON}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mavsdk/mavlink/v2.0/common"
)

# Minimal

file(GLOB MAVLINK_HEADERS_MINIMAL
   minimal/*.h
)

message("MAVLINK_HEADERS_MINIMAL are:")
message("${MAVLINK_HEADERS_MINIMAL}")

install(FILES
    ${MAVLINK_HEADERS_MINIMAL}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mavsdk/mavlink/v2.0/minimal"
)

# Piloting

file(GLOB MAVLINK_HEADERS_PILOTING
   piloting/*.h
)

message("MAVLINK_HEADERS_PILOTING are:")
message("${MAVLINK_HEADERS_PILOTING}")

install(FILES
    ${MAVLINK_HEADERS_PILOTING}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/mavsdk/mavlink/v2.0/piloting"
)