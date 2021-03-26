include_directories(
    ${PROJECT_SOURCE_DIR}/core
    SYSTEM  ${PROJECT_SOURCE_DIR}/mavlink/include
)

add_subdirectory(alarm)
add_subdirectory(alarm_robotic_vehicle)
add_subdirectory(command)
add_subdirectory(command_robotic_vehicle)
add_subdirectory(checklist)
add_subdirectory(checklist_robotic_vehicle)
add_subdirectory(hl_action)
add_subdirectory(hl_action_robotic_vehicle)
add_subdirectory(inspection)
add_subdirectory(inspection_robotic_vehicle)
add_subdirectory(telemetry)
add_subdirectory(telemetry_robotic_vehicle)