#pragma once

#include <memory>
#include "api/Signal.hpp"

class IMqtt;
class IMqttMessageDispatcher;
namespace spdlog {
    class logger;
}

class Termometer {
public:
    Termometer(const std::string& machineId, std::shared_ptr<IMqtt> mqtt, std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger);
    ~Termometer();
    double getTemperature() const;

    Signal<double>& onTemperatureChange(); 

private:
    void handleTemperature(const std::string& payload);
    void monitor();
    void stop();

    const std::string machineId_;
    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;
    std::shared_ptr<spdlog::logger> logger_;
    const std::string topic_; 
    double temperature_ {0.f};
    Signal<double> onTemperatureChange_;
};