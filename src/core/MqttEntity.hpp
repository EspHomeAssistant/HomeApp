#pragma once

#include <memory>
#include <spdlog/logger.h>
#include "api/Signal.hpp"
#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "core/DeviceType.hpp"

class MqttEntity {
public:
    MqttEntity(const std::string& machineId,
            std::shared_ptr<IMqtt> mqtt,
            std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher,
            std::shared_ptr<spdlog::logger> logger);
    virtual ~MqttEntity();

    std::string getMachineId() const;
    virtual DeviceType getDeviceType() const = 0;

private:
    virtual void handleMessage(const std::string& payload) = 0;
    void monitor();
    void stop();

protected:
    const std::string machineId_;
private:
    const std::string topic_;
    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;

protected:
    std::shared_ptr<spdlog::logger> logger_;
};

