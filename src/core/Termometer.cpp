#include "Termometer.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/logger.h>

Termometer::Type Termometer::getTemperature() const
{
    return getState();
}

Signal<Termometer::Type> &Termometer::onTemperatureChange()
{
    return onStateChange();
}

void Termometer::handleMessage(const std::string &payload)
try{
    using nlohmann::json;
    const json data = json::parse(payload);

    const bool temperatureUpdated {state_ != data["temperature"]};

    state_ = data["temperature"];
    logger_->info("Sensor {} raported temperature {}", machineId_, state_);

    if (temperatureUpdated) {
        onStateChange_.emit(state_);
    }

}catch(const nlohmann::json::parse_error& e) {
    logger_->warn(e.what());
}
