find_package(PkgConfig REQUIRED)
pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto REQUIRED) #??

add_library(mqtt-lib mqtt.cpp)
target_link_libraries(mqtt-lib PRIVATE PkgConfig::Mosquitto)
target_include_directories(mqtt-lib PRIVATE ${INCLUDE_DIR})


