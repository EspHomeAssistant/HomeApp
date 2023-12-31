#include "Registration.hpp"
#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include <nlohmann/json.hpp>
#include <spdlog/logger.h>
#include "MsgId.hpp"

Registration::Registration(std::shared_ptr<IMqtt> mqtt, std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger)
    : mqtt_{mqtt}
    , mqttMessageDispatcher_{mqttMessageDispatcher}
    , logger_{logger->clone("registration")}
{
    mqtt_->subscribe("/register/");
    //is it safe to pass this here?
    mqttMessageDispatcher_->on("/register/")->connect(this, &Registration::handleRegistration);
}

void Registration::handleRegistration(const std::string &payload)
try {
    using nlohmann::json;
    const json request = json::parse(payload);
    if (request["msgId"] != MsgId::RegistrationRequest) {
        return;
    }

    Registration::RegistrationId registrationId;

    const Registration::MachineId machineId {request["machineId"]}; 
    const bool hasBeenAlreadyRegistered {registeredDevices_.contains(machineId)};
    if (hasBeenAlreadyRegistered) {
        logger_->info("Device {} has been already registered with id {}", machineId, registeredDevices_[machineId]);
        registrationId = registeredDevices_[machineId];
    } else {
        registrationId = getRegistrationId();
        registeredDevices_[machineId] = registrationId;
        logger_->info("Device {} has been registered with id {}", machineId, registrationId);
    }

    const json response{
        {"machineId", machineId},
        {"status", "success"},
        {"registrationId", registrationId},
        {"msgId", MsgId::RegistrationResponse},
    };
 
    mqtt_->publish("/register/", response.dump());
    if (not hasBeenAlreadyRegistered) {
        onRegistration_.emit({machineId, request["sensorType"]});
    }

} catch(const nlohmann::json::parse_error& e) {
    logger_->warn(e.what());
}

Signal<RegisteredDevice>& Registration::onRegistration()
{
    return onRegistration_;
}

Registration::RegistrationId Registration::getRegistrationId() const
{
    static Registration::RegistrationId registrationId;
    return ++registrationId;
}