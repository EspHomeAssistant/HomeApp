#pragma once
#include <string>
#include <map>
#include <memory>
#include "api/Signal.hpp"


class MqttMessageDispatcher {
public:
    //how about string_view
    using Topic = std::string;

    void handle(const Topic& topic, const std::string& payload);
    std::shared_ptr<Signal<std::string>> on(const Topic&);

private:
    std::map<std::string, std::shared_ptr<Signal<std::string>>> handlers_;
};