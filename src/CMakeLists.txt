include(add_lib)

add_subdirectory(api)
add_subdirectory(core)

add_executable(example-main main.cpp)
target_include_directories(example-main PRIVATE
    ${INCLUDE_DIR}
)

target_link_libraries(example-main PRIVATE
    mqtt
    mqtt-message-dispatcher
    spdlog::spdlog
    registration
    registered-device
    registration
    mqtt-entity
    termometer
    relay-switch
    device-controller
)

add_custom_target(example-main-run
    COMMAND example-main
    DEPENDS example-main
)
