#include "RelaySwitch.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/logger.h>
#include <sstream>

#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"


RelaySwitch::RelaySwitch(const std::string &machineId, std::shared_ptr<IMqtt> mqtt, std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger)
    : machineId_{machineId}
    , mqtt_{mqtt}
    , mqttMessageDispatcher_{mqttMessageDispatcher}
    , logger_{logger->clone(machineId)}
    , topic_{"/sensor/" + machineId_ + "/"}
{
    monitor();
}

RelaySwitch::~RelaySwitch()
{
    stop();
}

std::vector<bool> RelaySwitch::getRelayState() const
{
    return relayState_;
}

Signal<std::vector<bool>> &RelaySwitch::onStateChange()
{
    return onStateChange_;
}

void RelaySwitch::handleRelay(const std::string &payload)
try{
    using nlohmann::json;
    const json data = json::parse(payload);

    const std::vector<bool> d = data["state"];
    const bool stateChanged {relayState_ != d};

    relayState_ = d;

    std::stringstream ss;
    ss << data["state"];

    logger_->info("Sensor {} raported relay state {}", machineId_, ss.str());

    if (stateChanged) {
        onStateChange_.emit(relayState_);
    }

}catch(const nlohmann::json::parse_error& e) {
    logger_->warn(e.what());
}

void RelaySwitch::monitor()
{
    mqtt_->subscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->connect(this, &RelaySwitch::handleRelay);
}

void RelaySwitch::stop()
{
    mqtt_->unsubscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->disconnect(this, &RelaySwitch::handleRelay);
}
