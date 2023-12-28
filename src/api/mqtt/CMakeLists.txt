find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto REQUIRED)

add_library(mqtt Mqtt.cpp)
target_link_libraries(mqtt 
    PkgConfig::Mosquitto
    spdlog::spdlog
)
target_include_directories(mqtt PRIVATE ${INCLUDE_DIR})

add_library(mqtt-message-dispatcher MqttMessageDispatcher.cpp)
target_include_directories(mqtt-message-dispatcher PRIVATE ${INCLUDE_DIR})





