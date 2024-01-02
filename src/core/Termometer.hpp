#pragma once

#include "api/Signal.hpp"
#include "core/MqttEntity.hpp"
#include "core/DeviceType.hpp"
#include "core/Stateful.hpp"

class Termometer : public MqttEntity, protected Stateful<double> {
public:
    using MqttEntity::MqttEntity;

    Type getTemperature() const;
    Signal<Type>& onTemperatureChange();

    DeviceType getDeviceType() const override;

private:
    void handleMessage(const std::string& payload) override;
};