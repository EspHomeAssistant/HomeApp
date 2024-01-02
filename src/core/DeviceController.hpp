#pragma once

#include <any>
#include <vector>
#include <memory>
#include "core/RegisteredDevice.hpp"
#include "core/MqttEntity.hpp"

namespace spdlog {
    class logger;
}
class IMqtt;
class IMqttMessageDispatcher;
class IRegistration;

class DeviceController {
public:
    DeviceController(std::shared_ptr<IRegistration> registration, std::shared_ptr<IMqtt> mqtt,
                  std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger);
    ~DeviceController();

    MqttEntity* getDevice(const std::string& machineId);

private:
    void onRegistration(const RegisteredDevice&);

    std::shared_ptr<IRegistration> registration_;
    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;
    std::shared_ptr<spdlog::logger> logger_;
    std::vector<std::unique_ptr<MqttEntity>> devices_;
};