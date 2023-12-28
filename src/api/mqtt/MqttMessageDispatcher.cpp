#include "api/mqtt/MqttMessageDispatcher.hpp"

void MqttMessageDispatcher::handle(const MqttMessageDispatcher::Topic& topic, const std::string& payload) {
    if (handlers_.contains(topic)) {
        handlers_.at(topic)->emit(payload);
    }
}

std::shared_ptr<Signal<std::string>> MqttMessageDispatcher::on(const MqttMessageDispatcher::Topic& topic) {
    if (not handlers_.contains(topic)) {
        handlers_.emplace(topic, std::make_shared<Signal<std::string>>());
    }
    return handlers_.at(topic);
}