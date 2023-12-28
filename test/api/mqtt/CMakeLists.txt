add_executable(mqtt-message-dispatcher-ut MqttMessageDispatcherTest.cpp)

target_include_directories(mqtt-message-dispatcher-ut PRIVATE 
    ${INCLUDE_DIR}
)

target_link_libraries(mqtt-message-dispatcher-ut
    PRIVATE
    gtest_main
    gtest
    gmock
    mqtt-message-dispatcher
)

gtest_discover_tests(mqtt-message-dispatcher-ut)

add_custom_target(mqtt-message-dispatcher-ut-run
    COMMAND mqtt-message-dispatcher-ut
    DEPENDS mqtt-message-dispatcher-ut
)
