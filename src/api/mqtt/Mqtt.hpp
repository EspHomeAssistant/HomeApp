#pragma once
#include <string>
#include <memory>
#include "api/Signal.hpp"

namespace spdlog 
{
    class logger;
}

class IMqtt {
public:
    virtual ~IMqtt() = default;
    virtual std::shared_ptr<spdlog::logger> getLogger() = 0;
    virtual Signal<std::string, std::string>& onMessage() = 0;
    virtual bool publish(const std::string& topic, const std::string& payload) = 0;
    virtual bool subscribe(const std::string& topic) = 0;
};

class Mqtt : public IMqtt {
public:
    Mqtt(const std::string& broker, std::shared_ptr<spdlog::logger> logger);
    ~Mqtt();
    Mqtt(const Mqtt&) = delete;
    Mqtt(Mqtt&&) = delete;
    Mqtt& operator=(const Mqtt&) = delete;
    Mqtt& operator=(Mqtt&&) = delete;

    std::shared_ptr<spdlog::logger> getLogger() override;
    Signal<std::string, std::string>& onMessage() override;
    bool publish(const std::string& topic, const std::string& payload) override;
    bool subscribe(const std::string& topic) override;

private:
    const std::string broker_;
    std::shared_ptr<spdlog::logger> logger_;
    Signal<std::string, std::string> onMessage_;
};