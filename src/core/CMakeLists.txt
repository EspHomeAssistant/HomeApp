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