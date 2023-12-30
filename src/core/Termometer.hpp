#pragma once

#include "api/Signal.hpp"
#include "core/MqttEntity.hpp"

class Termometer : public MqttEntity<double> {
public:
    using MqttEntity::MqttEntity;

    Type getTemperature() const;
    Signal<Type>& onTemperatureChange();

private:
    void handleMessage(const std::string& payload) override;
};