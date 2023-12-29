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
    logger_->info("Received register request {}",payload);
    using nlohmann::json;
    const json request = json::parse(payload);
    if (request["msgId"] != MsgId::RegistrationRequest) {
        return;
    }

    const Registration::MachineId machineId {request["machineId"]}; 
    if (registeredDevices_.contains(machineId)) {
        logger_->info("Device {} has been already registered with id {}", machineId, registeredDevices_[machineId]);
        return;
    }

    Registration::RegistrationId registrationId = getRegistrationId();

    json response;
    response["machineId"] = request["machineId"];
    response["status"] = "success";
    response["registrationId"] = registrationId;
    response["msgId"] = MsgId::RegistrationResponse;
    
    registeredDevices_["machineId"] = registrationId;

    mqtt_->publish("/register/", response.dump());

    logger_->info("Device {} has been registered with id {}", machineId, registrationId);


} catch(const nlohmann::json::parse_error& e) {
    logger_->warn(e.what());
}

Registration::RegistrationId Registration::getRegistrationId() const
{
    static Registration::RegistrationId registrationId;
    return ++registrationId;
}