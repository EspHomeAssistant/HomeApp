#include "MqttEntity.hpp"

#include <utility>

MqttEntity::MqttEntity( 
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

MqttEntity::~MqttEntity()
{
    stop();
}

MqttEntity::MqttEntity(MqttEntity&& other)
{
    other.stop();

    other.shouldUnsubscribe = false;
    machineId_ = std::exchange(other.machineId_, "");
    topic_ = std::exchange(other.topic_, "");
    mqtt_ = std::exchange(other.mqtt_, nullptr);
    mqttMessageDispatcher_ = std::exchange(other.mqttMessageDispatcher_, nullptr);
    logger_ = std::exchange(other.logger_, nullptr);

    monitor();
}

MqttEntity &MqttEntity::operator=(MqttEntity && other)
{
    other.stop();

    other.shouldUnsubscribe = false;
    machineId_ = std::exchange(other.machineId_, "");
    topic_ = std::exchange(other.topic_, "");
    mqtt_ = std::exchange(other.mqtt_, nullptr);
    mqttMessageDispatcher_ = std::exchange(other.mqttMessageDispatcher_, nullptr);
    logger_ = std::exchange(other.logger_, nullptr);

    monitor();
    return *this;
}

std::string MqttEntity::getMachineId() const
{
    return machineId_;
}

void MqttEntity::monitor()
{
    mqtt_->subscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->connect(this, &MqttEntity::handleMessage);
}

void MqttEntity::stop()
{
    if (shouldUnsubscribe) {
        mqtt_->unsubscribe(topic_);
        mqttMessageDispatcher_->on(topic_)->disconnect(this, &MqttEntity::handleMessage);
    }
}

void MqttEntity::sendRequest(const std::string& payload)
{
    mqtt_->publish(topic_, payload);
}