#include "api/mqtt/Mqtt.hpp"
#include <mosquitto.h>

#include <spdlog/logger.h>
#include "Mqtt.hpp"

namespace {
    mosquitto* mqtt_{nullptr};

    void callback(mosquitto* mqtt_, void* obj, const mosquitto_message* msg) {
        if (not (mqtt_ && obj && msg)) {
            return;
        }
        auto self = reinterpret_cast<Mqtt*>(obj);
        auto logger = self->getLogger();
        const std::string payload{static_cast<const char*>(msg->payload), static_cast<size_t>(msg->payloadlen)};
        logger->info("Received topic: {} payload: {}", msg->topic, payload);
        self->onMessage().emit(msg->topic, payload);
    }
}

Mqtt::Mqtt(const std::string& broker, std::shared_ptr<spdlog::logger> logger) 
    : broker_{broker}
    , logger_{logger}
    {
    const int isInitialized = mosquitto_lib_init();
    int major, minor, revision;
    mosquitto_lib_version(&major, &minor, &revision);
    logger_->info("Mosguitto version: {}.{}.{} isInitialized: {}", major, minor, revision, (isInitialized == MOSQ_ERR_SUCCESS)? "true" : "false");
    mqtt_ = mosquitto_new(NULL, true, this);
    
    const int isConnected = mosquitto_connect_async(mqtt_, broker_.c_str(), 1883, 30);
    logger_->info("Connection to {} {}", broker_, isConnected == MOSQ_ERR_SUCCESS ? "succeed" : "failed");
    
    mosquitto_message_callback_set(mqtt_, callback);
    mosquitto_loop_start(mqtt_);
}

Mqtt::~Mqtt(){
    mosquitto_loop_stop(mqtt_, true);
    mosquitto_disconnect(mqtt_);
    mosquitto_destroy(mqtt_);
    mqtt_ = nullptr;
    mosquitto_lib_cleanup();
}

std::shared_ptr<spdlog::logger> Mqtt::getLogger()
{
    return logger_;
}

Signal<std::string, std::string>& Mqtt::onMessage()
{
    return onMessage_;
}

bool Mqtt::publish(const std::string &topic, const std::string &payload)
{
    const int sendingStatus = mosquitto_publish(mqtt_, NULL, topic.c_str(), payload.length() + 1, payload.c_str(), 0, false);
    const bool isSubscribed {MOSQ_ERR_SUCCESS == sendingStatus};
    logger_->info("Msg send topic: {} payload: {} status: {}", topic, payload, isSubscribed ? "success" : "fail");
    return isSubscribed;
}

bool Mqtt::subscribe(const std::string &topic)
{
    const int subscriptionStatus = mosquitto_subscribe(mqtt_, NULL, topic.c_str(), 0);
    const bool isSubscribed {MOSQ_ERR_SUCCESS == subscriptionStatus};
    logger_->info("Subscription to {} {}", topic, isSubscribed ? "succeed" : "failed");
    return isSubscribed;
}

bool Mqtt::unsubscribe(const std::string &topic)
{
    const int unsubscriptionStatus = mosquitto_unsubscribe(mqtt_, NULL, topic.c_str());
    const bool isUnsubscribed {MOSQ_ERR_SUCCESS == unsubscriptionStatus};
    logger_->info("Unubscription from {} {}", topic, isUnsubscribed ? "succeed" : "failed");
    return isUnsubscribed;
}
