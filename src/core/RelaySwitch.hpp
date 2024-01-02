#pragma once

#include "api/Signal.hpp"
#include "core/MqttEntity.hpp"
#include "core/DeviceType.hpp"
#include "core/Stateful.hpp"

class RelaySwitch : public MqttEntity, protected Stateful<std::vector<bool>>{
public:
    using MqttEntity::MqttEntity;

    Type getRelayState() const;
    Signal<Type>& onStateChange(); 

    DeviceType getDeviceType() const override;

private:
    void handleMessage(const std::string& payload) override;
};