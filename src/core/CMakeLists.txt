add_lib(NAME registration 
        SOURCES Registration.cpp
        DEPENDS
            spdlog::spdlog
            mqtt
            mqtt-message-dispatcher
            nlohmann_json::nlohmann_json
        )

add_lib(NAME termometer 
SOURCES Termometer.cpp
DEPENDS
    spdlog::spdlog
    mqtt
    mqtt-message-dispatcher
    nlohmann_json::nlohmann_json
)

add_lib(NAME relay-switch 
SOURCES RelaySwitch.cpp
DEPENDS
    spdlog::spdlog
    mqtt
    mqtt-message-dispatcher
    nlohmann_json::nlohmann_json
)

add_library(mqtt-entity INTERFACE)
target_link_libraries(mqtt-entity INTERFACE
    spdlog::spdlog
    mqtt
    mqtt-message-dispatcher
)