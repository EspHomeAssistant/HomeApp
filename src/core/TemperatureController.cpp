#include "TemperatureController.hpp"

#include <algorithm>
#include <spdlog/logger.h>

TemperatureController::TemperatureController(std::shared_ptr<Termometer> termometer,
                                             std::shared_ptr<RelaySwitch> relaySwitch,
                                             std::shared_ptr<spdlog::logger> logger)
    : termometer_{termometer}
    , relaySwitch_{relaySwitch}
    , logger_{logger->clone("TemperatureController")}
    , hysteresis {0.5}
    , demandTemperature {22.5}
{
}   

void TemperatureController::control()
{
    const auto currentTemperature = termometer_->getTemperature();
    const auto currentRelayState = relaySwitch_->getRelayState();
    if (currentTemperature <= demandTemperature - hysteresis) {
        if (std::ranges::all_of(currentRelayState, [](const bool state){ return state == true;})) {
            logger_->info("Temperature {} too low. Keep relay on.", currentTemperature);
        }
        else {
            relaySwitch_->on();
            logger_->info("Temperature {} too low. Turn on relay.", currentTemperature);
        }
    } else if (currentTemperature >= demandTemperature + hysteresis) {
        if (std::ranges::all_of(currentRelayState, [](const bool state){ return state == false;})) {
            logger_->info("Temperature {} too high. Keep relay off.", currentTemperature);
        }
        else {
            relaySwitch_->off();
            logger_->info("Temperature {} too high. Turn off relay.", currentTemperature);
        }
    }
}
