add_library(registration Registration.cpp)
target_link_libraries(registration PRIVATE
    spdlog::spdlog
    mqtt
    mqtt-message-dispatcher
)
target_include_directories(registration PRIVATE ${INCLUDE_DIR})
