#pragma once

#include "api/Signal.hpp"
#include "core/MqttEntity.hpp"
#include "core/DeviceType.hpp"

class RelaySwitch : public MqttEntity<std::vector<bool>>{
public:
    using MqttEntity::MqttEntity;

    Type getRelayState() const;
    Signal<Type>& onStateChange(); 

    DeviceType getDeviceType() const override;

private:
    void handleMessage(const std::string& payload) override;
};