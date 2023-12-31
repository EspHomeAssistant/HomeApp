#include "RelaySwitch.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/logger.h>
#include <sstream>

RelaySwitch::Type RelaySwitch::getRelayState() const
{
    return getState();
}

Signal<RelaySwitch::Type> &RelaySwitch::onStateChange()
{
    return MqttEntity::onStateChange();
}

DeviceType RelaySwitch::getDeviceType() const
{
    return DeviceType::Relay;
}

void RelaySwitch::handleMessage(const std::string &payload)
try{
    using nlohmann::json;
    const json data = json::parse(payload);

    const std::vector<bool> d = data["state"];
    const bool stateChanged {state_ != d};

    state_ = d;

    std::stringstream ss;
    ss << data["state"];

    logger_->info("Sensor {} raported relay state {}", machineId_, ss.str());

    if (stateChanged) {
        onStateChange_.emit(state_);
    }

}catch(const nlohmann::json::parse_error& e) {
    logger_->warn(e.what());
}