#pragma once

#include <any>
#include <vector>
#include <memory>
#include "core/Registration.hpp"
#include "core/RegisteredDevice.hpp"

namespace spdlog {
    class logger;
}
class IMqtt;
class IMqttMessageDispatcher;

class DeviceController {
public:
    DeviceController(std::shared_ptr<Registration> registration, std::shared_ptr<IMqtt> mqtt,
                  std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger);
    ~DeviceController();

private:
    void onRegistration(const RegisteredDevice&);

    std::shared_ptr<Registration> registration_;
    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;
    std::shared_ptr<spdlog::logger> logger_;
    std::vector<std::shared_ptr<MqttEntity>> devices_;
};