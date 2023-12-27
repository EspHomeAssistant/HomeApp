#include "api/mqtt/Mqtt.hpp"
#include <mosquitto.h>

#include <spdlog/logger.h>

namespace {
    mosquitto* mqtt_{nullptr};

    void callback(mosquitto* mqtt_, void* obj, const mosquitto_message* msg) {
        if (not (mqtt_ && obj && msg)) {
            return;
        }
        auto self = reinterpret_cast<Mqtt*>(obj);
        auto logger = self->getLogger();
        logger->info("Received topic: {} payload: {}", msg->topic, std::string{static_cast<const char*>(msg->payload), static_cast<size_t>(msg->payloadlen)});
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

    const int isSubscribed = mosquitto_subscribe(mqtt_, NULL, "test", 0);
    logger_->info("Subscription to {} {}", "test", isSubscribed == MOSQ_ERR_SUCCESS ? "succeed" : "failed");

    mosquitto_loop_start(mqtt_);

    for (int i = 0; i < 10; ++i) {
        const std::string topic {"test"};
        const std::string payload {"test message "+ std::to_string(i)};
        const int msgSent = mosquitto_publish(mqtt_, NULL, topic.c_str(), payload.length() + 1, payload.c_str(), 0, false);
        logger_->info("Msg send topic: {} payload: {} status: {}", topic, payload, msgSent == MOSQ_ERR_SUCCESS ? "success" : "fail");
    }
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