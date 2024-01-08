#pragma once

#include <memory>

#include "core/Termometer.hpp"
#include "core/RelaySwitch.hpp"

namespace sdplog {
    class logger;
}

class TemperatureController{
public:
    TemperatureController(std::shared_ptr<Termometer> termometer, std::shared_ptr<RelaySwitch> relaySwitch, std::shared_ptr<spdlog::logger> logger);
    void control();

private:
    std::shared_ptr<Termometer> termometer_;
    std::shared_ptr<RelaySwitch> relaySwitch_;
    std::shared_ptr<spdlog::logger> logger_;
    double hysteresis;
    double demandTemperature;
};