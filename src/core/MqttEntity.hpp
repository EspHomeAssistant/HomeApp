#pragma once

#include <memory>
#include "api/Signal.hpp"

class IMqtt;
class IMqttMessageDispatcher;
namespace spdlog {
    class logger;
}

template<class State>
class MqttEntity {
public:
    using Type = State;
    MqttEntity(
            const std::string& machineId,
            std::shared_ptr<IMqtt> mqtt,
            std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher,
            std::shared_ptr<spdlog::logger> logger)
            : machineId_{machineId}
            , topic_{"/sensor/" + machineId_ + "/"}
            , mqtt_{mqtt}
            , mqttMessageDispatcher_{mqttMessageDispatcher}
            , logger_{logger}
            {
                monitor();
            }
    virtual ~MqttEntity() {
        stop();
    }

    State getState() const;
    Signal<State>& onStateChange();

private:
    virtual void handleMessage(const std::string& payload) = 0;
    void monitor();
    void stop();

    const std::string machineId_;
    const std::string topic_;
    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;
    std::shared_ptr<spdlog::logger> logger_;
    Signal<State> onStateChange_;
    State state_;
};

template <class State>
inline State MqttEntity<State>::getState() const
{
    return state_;
}

template <class State>
inline Signal<State> &MqttEntity<State>::onStateChange()
{
    return onStateChange_;
}

template <class State>
inline void MqttEntity<State>::monitor()
{
    mqtt_->subscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->connect(this, &MqttEntity::handleMessage);
}

template <class State>
inline void MqttEntity<State>::stop()
{
    mqtt_->unsubscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->disconnect(this, &MqttEntity::handleMessage);
}
