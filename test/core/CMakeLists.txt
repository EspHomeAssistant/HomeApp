add_ut(NAME registration
       SOURCES RegistrationTest.cpp
)

add_ut(NAME termometer
       SOURCES TermometerTest.cpp
)

add_ut(NAME relay-switch 
       SOURCES RelaySwitchTest.cpp
)

add_ut(NAME mqtt-entity
       SOURCES MqttEntityTest.cpp  
)

add_ut(NAME registered-device 
       SOURCES RegisteredDeviceTest.cpp
)

add_ut(NAME stateful
       SOURCES StatefulTest.cpp
       HEADER_ONLY TRUE
       )