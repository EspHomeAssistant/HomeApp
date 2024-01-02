#include "DeviceController.hpp"

#include <algorithm>

#include <spdlog/logger.h>
#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "core/Registration.hpp"
#include "core/Termometer.hpp"
#include "core/RelaySwitch.hpp"
#include "core/DeviceType.hpp"

DeviceController::DeviceController(std::shared_ptr<IRegistration> registration, std::shared_ptr<IMqtt> mqtt, std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger)
    : registration_{registration}
    , mqtt_{mqtt}
    , mqttMessageDispatcher_{mqttMessageDispatcher}
    , logger_{logger->clone("DeviceController")}
{
    registration_->onRegistration().connect(this, &DeviceController::onRegistration);
}

DeviceController::~DeviceController()
{
    registration_->onRegistration().disconnect(this, &DeviceController::onRegistration);
}

MqttEntity* DeviceController::getDevice(const std::string &machineId)
{   
    auto hasMachineId = [&machineId](const auto& device){
        return device->getMachineId() == machineId;
    };
    const auto found = std::ranges::find_if(devices_, hasMachineId);
    if (found != std::end(devices_)) {
        return found->get();
    }
    return nullptr;
}

void DeviceController::onRegistration(const RegisteredDevice& device)
{
    if(device.type == DeviceType::Temperature) {
        devices_.emplace_back(std::make_unique<Termometer>(device.machineId, mqtt_, mqttMessageDispatcher_, logger_));
        logger_->info("Constructed Termometer with id {}", device.machineId);  
    } else if (device.type == DeviceType::Relay) {
        devices_.emplace_back(std::make_unique<RelaySwitch>(device.machineId, mqtt_, mqttMessageDispatcher_, logger_));
        logger_->info("Constructed RelaySwitch with id {}", device.machineId);
    } else {
        logger_->info("Cannot construct device with id {} and type {}", device.machineId, static_cast<int>(device.type));
    }
}
