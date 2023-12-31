#pragma once

#include "api/Signal.hpp"
#include "core/MqttEntity.hpp"
#include "core/DeviceType.hpp"

class Termometer : public MqttEntity<double> {
public:
    using MqttEntity::MqttEntity;

    Type getTemperature() const;
    Signal<Type>& onTemperatureChange();

    DeviceType getDeviceType() const override;

private:
    void handleMessage(const std::string& payload) override;
};