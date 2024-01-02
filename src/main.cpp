#include <iostream>
#include <thread>
#include <chrono>

#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "core/Registration.hpp"
#include "core/DeviceController.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "nlohmann/json.hpp"

int main() {
    using nlohmann::json;

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/basic-log.txt");
    auto logger = std::make_shared<spdlog::logger>("main", file_sink);
    logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());

    const std::string registrationTopic {"/register/"};

    std::shared_ptr<IMqtt> mqtt = std::make_shared<Mqtt>("127.0.0.1", logger->clone("mqtt"));
    std::shared_ptr<IMqttMessageDispatcher> dispatcher = std::make_shared<MqttMessageDispatcher>();
    mqtt->onMessage().connect(dispatcher.get(), &IMqttMessageDispatcher::handle);
    
    std::shared_ptr<Registration> registration = std::make_shared<Registration>(mqtt, dispatcher, logger);
    DeviceController controller{registration, mqtt, dispatcher, logger};

    std::this_thread::sleep_for(std::chrono::seconds(120));
    return 0;
}
