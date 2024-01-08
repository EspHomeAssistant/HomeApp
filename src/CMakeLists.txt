include(add_lib)

add_subdirectory(api)
add_subdirectory(core)

add_executable(example-main main.cpp)
target_include_directories(example-main PRIVATE
    ${INCLUDE_DIR}
)

find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

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
    temperature-controller
    Boost::system    

    mongo::bsoncxx_shared
    mongo::mongocxx_shared
)

add_custom_target(example-main-run
    COMMAND example-main
    DEPENDS example-main
)
