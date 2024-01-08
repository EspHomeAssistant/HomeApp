#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "core/Registration.hpp"
#include "core/DeviceController.hpp"
#include "core/TemperatureController.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <nlohmann/json.hpp>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

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

    std::this_thread::sleep_for(std::chrono::seconds(60));

    auto termometer = dynamic_pointer_cast<Termometer>(controller.getDevice("e6:5d:b7:0")); 
    auto relay = dynamic_pointer_cast<RelaySwitch>(controller.getDevice("63:b7:b8:0"));
    TemperatureController tempControl{termometer, relay, logger};
    
    boost::asio::io_context io;
    boost::asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto){ io.stop(); });

    boost::asio::steady_timer t{io, boost::asio::chrono::seconds(20)};
    
    std::function<void(const boost::system::error_code&)> wait = [&](const boost::system::error_code&){
        tempControl.control();
        t.expires_at(boost::asio::chrono::steady_clock::now() + boost::asio::chrono::seconds(20));
        t.async_wait(wait);
    };
    
    t.async_wait(wait);

    io.run();
    return 0;
}
