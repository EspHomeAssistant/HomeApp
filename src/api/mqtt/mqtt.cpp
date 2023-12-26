#include "api/mqtt/mqtt.hpp"
#include <mosquitto.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <format>

namespace {
    mosquitto* mqtt_{nullptr};

    void callback(mosquitto* mqtt_, void* obj, const mosquitto_message* msg) {
        std::cout << "This is called from: 0x" << std::hex << std::this_thread::get_id() << std::endl;
        std::cout << "Received {topic: " << msg->topic << " payload: " << std::string(static_cast<const char*>(msg->payload), msg->payloadlen) << "}" << std::endl;
    }
}

Mqtt::Mqtt(const std::string& ) {
    const int isInitialized = mosquitto_lib_init();
    int major, minor, revision;
    mosquitto_lib_version(&major, &minor, &revision);
    std::cout << std::format("Mosguitto version: {}.{}.{} isInitialized: {}", major, minor, revision, (isInitialized == MOSQ_ERR_SUCCESS)? "true" : "false") << std::endl;
    mqtt_ = mosquitto_new(NULL, true, this);

    const int isConnected = mosquitto_connect(mqtt_, "127.0.0.1", 1883, 30);
    std::cout << "isConnected: " << std::boolalpha << (isConnected == MOSQ_ERR_SUCCESS) << std::endl;

    std::cout << "This is called from: 0x" << std::hex << std::this_thread::get_id() << std::endl;
    
    mosquitto_message_callback_set(mqtt_, callback);

    const int isSubscribed = mosquitto_subscribe(mqtt_, NULL, "test", 0);
    std::cout << "Subscribed to topic: test successfully: " << std::boolalpha << (isSubscribed == MOSQ_ERR_SUCCESS) << std::endl;

    for (int i = 0; i < 10; ++i) {
        const int msgSent = mosquitto_publish(mqtt_, NULL, "test", 10, "hellotest", 0, false);
        std::cout << "msgSent: " << (msgSent  == MOSQ_ERR_SUCCESS) << std::endl;
        mosquitto_loop(	mqtt_, 0, 1);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

Mqtt::~Mqtt(){
    mosquitto_destroy(mqtt_);
    mqtt_ = nullptr;
    mosquitto_lib_cleanup();
}