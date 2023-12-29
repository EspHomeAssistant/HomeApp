find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto REQUIRED)

add_lib(NAME mqtt
        SOURCES Mqtt.cpp
        DEPENDS
            PkgConfig::Mosquitto
            spdlog::spdlog
        )

add_lib(NAME mqtt-message-dispatcher
        SOURCES MqttMessageDispatcher.cpp
        )