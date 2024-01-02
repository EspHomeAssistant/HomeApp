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
    MqttEntity(const MqttEntity&) = delete;
    MqttEntity(MqttEntity&&);
    MqttEntity& operator=(const MqttEntity&) = delete;
    MqttEntity& operator=(MqttEntity&&);    

    std::string getMachineId() const;
    virtual DeviceType getDeviceType() const = 0;

protected:
    void sendRequest(const std::string& request);

private:
    virtual void handleMessage(const std::string& payload) = 0;
    void monitor();
    void stop();

protected:
    std::string machineId_;

private:
    std::string topic_;
    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;

protected:
    std::shared_ptr<spdlog::logger> logger_;

private:
    bool shouldUnsubscribe{true};

};

