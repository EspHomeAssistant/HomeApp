add_lib(NAME registration 
        SOURCES Registration.cpp
        DEPENDS
            spdlog::spdlog
            mqtt
            mqtt-message-dispatcher
        )
