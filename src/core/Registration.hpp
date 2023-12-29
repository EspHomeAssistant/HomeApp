#pragma once

#include <memory>
#include <map>

class IMqttMessageDispatcher;
class IMqtt;
namespace spdlog {
    class logger;
}

class Registration
{
public:
    Registration(std::shared_ptr<IMqtt> mqtt, std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger);

    void handleRegistration(const std::string& payload);

private:
    using MachineId = std::string;
    using RegistrationId = int;

    RegistrationId getRegistrationId() const;

    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;
    std::map<MachineId, RegistrationId> registeredDevices_;
    std::shared_ptr<spdlog::logger> logger_;
};