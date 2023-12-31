#pragma once

#include <memory>
#include <type_traits>
#include <spdlog/logger.h>
#include "api/Signal.hpp"
#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "core/DeviceType.hpp"

template <class T>
concept DefaultConstructible = std::is_default_constructible<T>::value;

template<DefaultConstructible State>
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
            , logger_{logger->clone(machineId)}
            {
                monitor();
            }
    virtual ~MqttEntity() {
        stop();
    }

    std::string getMachineId() const;
    virtual DeviceType getDeviceType() const = 0;

protected:
    State getState() const;
    Signal<State>& onStateChange();

private:
    virtual void handleMessage(const std::string& payload) = 0;
    void monitor();
    void stop();

protected:
    const std::string machineId_;
private:
    const std::string topic_;
    std::shared_ptr<IMqtt> mqtt_;
    std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher_;

protected:
    std::shared_ptr<spdlog::logger> logger_;
    Signal<State> onStateChange_;
    State state_{};
};

template <DefaultConstructible State>
inline std::string MqttEntity<State>::getMachineId() const
{
    return machineId_;
}

template <DefaultConstructible State>
inline State MqttEntity<State>::getState() const
{
    return state_;
}

template <DefaultConstructible State>
inline Signal<State> &MqttEntity<State>::onStateChange()
{
    return onStateChange_;
}

template <DefaultConstructible State>
inline void MqttEntity<State>::monitor()
{
    mqtt_->subscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->connect(this, &MqttEntity::handleMessage);
}

template <DefaultConstructible State>
inline void MqttEntity<State>::stop()
{
    mqtt_->unsubscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->disconnect(this, &MqttEntity::handleMessage);
}

