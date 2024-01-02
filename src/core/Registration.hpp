#pragma once

#include <memory>
#include <map>
#include "api/Signal.hpp"
#include "core/RegisteredDevice.hpp"

class IMqttMessageDispatcher;
class IMqtt;
namespace spdlog {
    class logger;
}

class IRegistration {
public:
    virtual ~IRegistration() = default;
    virtual void handleRegistration(const std::string& payload) = 0;
    virtual Signal<RegisteredDevice>& onRegistration() = 0;
};

class Registration: public IRegistration
{
public:
    Registration(std::shared_ptr<IMqtt> mqtt, std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger);

    void handleRegistration(const std::string& payload) override;
    Signal<RegisteredDevice>& onRegistration() override;

private:
    using MachineId = std::string;
    using RegistrationId = int;

    RegistrationId getRegistrationId() const;

    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;
    std::map<MachineId, RegistrationId> registeredDevices_;
    std::shared_ptr<spdlog::logger> logger_;
    Signal<RegisteredDevice> onRegistration_;
};