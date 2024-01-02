#include "RelaySwitch.hpp"

#include <algorithm>
#include <sstream>
#include <nlohmann/json.hpp>
#include <spdlog/logger.h>
#include <core/MsgId.hpp>

RelaySwitch::Type RelaySwitch::getRelayState() const
{
    return getState();
}

Signal<RelaySwitch::Type> &RelaySwitch::onStateChange()
{
    return Stateful::onStateChange();
}

void RelaySwitch::on()
{
    //@todo for now lets consider all outputs as a switch which controls the same area
    Type request;
    std::ranges::transform(state_, std::back_inserter(request), [](const bool){
        return true;
    });
    requestState(request);
}

void RelaySwitch::off()
{
    Type request;
    std::ranges::transform(state_, std::back_inserter(request), [](const bool){
        return false;
    });
    requestState(request);
}

void RelaySwitch::requestState(const RelaySwitch::Type& state)
{
    using nlohmann::json;
    const json request {
        {"msgId", MsgId::RelayStateRequest},
        {"state", state}
    };

    std::stringstream ss;
    ss << request["state"];

    sendRequest(request.dump());
    logger_->info("Sensor {} requested state: {}", machineId_, ss.str());
}

DeviceType RelaySwitch::getDeviceType() const
{
    return DeviceType::Relay;
}

void RelaySwitch::handleMessage(const std::string &payload)
try{
    using nlohmann::json;
    const json data = json::parse(payload);

    if (data["msgId"] != MsgId::RelayStateNotif) {
        return;
    }

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