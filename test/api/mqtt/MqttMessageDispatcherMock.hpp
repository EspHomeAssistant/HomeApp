#pragma once

#include <gmock/gmock.h>
#include "api/mqtt/MqttMessageDispatcher.hpp"

class MqttMessageDispatcherMock : public IMqttMessageDispatcher {
public:
    MOCK_METHOD(void, handle, (const Topic& topic, const std::string& payload));
    MOCK_METHOD(std::shared_ptr<Signal<std::string>>, on,(const Topic&));
};