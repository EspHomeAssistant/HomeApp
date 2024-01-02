#include "MqttEntity.hpp"

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
    mqtt_->unsubscribe(topic_);
    mqttMessageDispatcher_->on(topic_)->disconnect(this, &MqttEntity::handleMessage);
}