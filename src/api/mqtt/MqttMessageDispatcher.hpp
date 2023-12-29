#pragma once
#include <string>
#include <map>
#include <memory>
#include "api/Signal.hpp"

class IMqttMessageDispatcher {
public:
    using Topic = std::string;

    virtual ~IMqttMessageDispatcher() = default;
    virtual void handle(const Topic& topic, const std::string& payload) = 0;
    virtual std::shared_ptr<Signal<std::string>> on(const Topic&) = 0;
};

class MqttMessageDispatcher : public IMqttMessageDispatcher {
public:
    void handle(const Topic& topic, const std::string& payload) override;
    std::shared_ptr<Signal<std::string>> on(const Topic&) override;

private:
    std::map<std::string, std::shared_ptr<Signal<std::string>>> handlers_;
};