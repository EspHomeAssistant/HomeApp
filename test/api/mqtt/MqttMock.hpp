#pragma once

#include <gmock/gmock.h>
#include "api/mqtt/Mqtt.hpp"

class MqttMock : public IMqtt {
public:
    MOCK_METHOD(bool, publish, (const std::string& topic, const std::string& payload));
    MOCK_METHOD(bool, subscribe, (const std::string& topic));
    MOCK_METHOD(bool, unsubscribe, (const std::string& topic));
    MOCK_METHOD(std::shared_ptr<spdlog::logger>, getLogger, ());
    MOCK_METHOD((Signal<std::string, std::string>&), onMessage, ());
};
