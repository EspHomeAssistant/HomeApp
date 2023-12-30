#pragma once

#include <memory>
#include "api/Signal.hpp"

class IMqtt;
class IMqttMessageDispatcher;
namespace spdlog {
    class logger;
}

class RelaySwitch {
public:
    RelaySwitch(const std::string& machineId, std::shared_ptr<IMqtt> mqtt, std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher, std::shared_ptr<spdlog::logger> logger);
    ~RelaySwitch();

    std::vector<bool> getRelayState() const;
    Signal<std::vector<bool>>& onStateChange(); 

private:
    void handleRelay(const std::string &payload);
    void monitor();
    void stop();

    const std::string machineId_;
    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;
    std::shared_ptr<spdlog::logger> logger_;
    const std::string topic_;
    std::vector<bool> relayState_; 
    Signal<std::vector<bool>> onStateChange_;
};