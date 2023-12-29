#include <iostream>
#include <thread>
#include <chrono>

#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "core/Registration.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "nlohmann/json.hpp"

int main() {
    using nlohmann::json;

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/basic-log.txt");
    auto logger = std::make_shared<spdlog::logger>("main", file_sink);
    auto mqttLogger = std::make_shared<spdlog::logger>("mqtt", file_sink);

    const std::string registrationTopic {"/register/"};

    std::shared_ptr<IMqtt> mqtt = std::make_shared<Mqtt>("127.0.0.1", mqttLogger);
    std::shared_ptr<IMqttMessageDispatcher> dispatcher = std::make_shared<MqttMessageDispatcher>();
    mqtt->onMessage().connect(dispatcher.get(), &IMqttMessageDispatcher::handle);
    Registration registration{mqtt, dispatcher, logger};

    std::this_thread::sleep_for(std::chrono::seconds(30));
    return 0;
}
