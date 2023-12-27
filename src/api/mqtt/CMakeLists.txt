find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto REQUIRED)

add_library(mqtt-lib Mqtt.cpp)

target_link_libraries(mqtt-lib 
    PkgConfig::Mosquitto
    spdlog::spdlog
)

target_include_directories(mqtt-lib PRIVATE ${INCLUDE_DIR})


