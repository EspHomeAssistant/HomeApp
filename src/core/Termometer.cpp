#include "Termometer.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/logger.h>

#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"


Termometer::Termometer(const std::string &machineId, std::shared_ptr<IMqtt> mqtt, std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger)
    : machineId_{machineId}
    , mqtt_{mqtt}
    , mqttMessageDispatcher_{mqttMessageDispatcher}
    , logger_{logger->clone(machineId)}
    , topic_{"/sensor/" + machineId_ + "/"}
{
    monitor();
}

Termometer::~Termometer()
{
    stop();
}

double Termometer::getTemperature() const
{
    return temperature_;
}

Signal<double> &Termometer::onTemperatureChange()
{
    return onTemperatureChange_;
}

void Termometer::handleTemperature(const std::string &payload)
try{
    using nlohmann::json;
    const json data = json::parse(payload);

    const bool temperatureUpdated {temperature_ != data["temperature"]};

    temperature_ = data["temperature"];
    logger_->info("Sensor {} raported temperature {}", machineId_, temperature_);

    if (temperatureUpdated) {
        onTemperatureChange_.emit(temperature_);
    }

}catch(const nlohmann::json::parse_error& e) {
    logger_->warn(e.what());
}

void Termometer::monitor()
{
    mqtt_->subscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->connect(this, &Termometer::handleTemperature);
}

void Termometer::stop()
{
    mqtt_->unsubscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->disconnect(this, &Termometer::handleTemperature);
}
