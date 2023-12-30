#pragma once

#include "api/Signal.hpp"
#include "core/MqttEntity.hpp"

class RelaySwitch : public MqttEntity<std::vector<bool>>{
public:
    using MqttEntity::MqttEntity;

    Type getRelayState() const;
    Signal<Type>& onStateChange(); 

private:
    void handleMessage(const std::string& payload) override;
};