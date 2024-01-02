add_lib(NAME registered-device 
SOURCES RegisteredDevice.cpp
)

add_lib(NAME registration 
SOURCES Registration.cpp
DEPENDS
    spdlog::spdlog
    mqtt
    mqtt-message-dispatcher
    nlohmann_json::nlohmann_json
    registered-device
)

add_lib(NAME termometer 
SOURCES Termometer.cpp
DEPENDS
    spdlog::spdlog
    mqtt
    mqtt-message-dispatcher
    nlohmann_json::nlohmann_json
    mqtt-entity
)

add_lib(NAME relay-switch 
SOURCES RelaySwitch.cpp
DEPENDS
    spdlog::spdlog
    mqtt
    mqtt-message-dispatcher
    nlohmann_json::nlohmann_json
    mqtt-entity
)

add_lib(NAME mqtt-entity
SOURCES MqttEntity.cpp
DEPENDS
    spdlog::spdlog
    mqtt
    mqtt-message-dispatcher
    nlohmann_json::nlohmann_json
)

add_lib(NAME device-controller
SOURCES DeviceController.cpp
DEPENDS
    spdlog::spdlog
    mqtt
    mqtt-message-dispatcher
    relay-switch
    termometer
)
