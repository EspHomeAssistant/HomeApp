#include <iostream>
#include <thread>
#include <chrono>

#include "api/mqtt/Mqtt.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

int main() {
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/basic-log.txt");
    auto logger = std::make_shared<spdlog::logger>("main", file_sink);
    auto mqttLogger = std::make_shared<spdlog::logger>("mqtt", file_sink);
    logger->info("Hello world");

    auto mqtt = Mqtt{"127.0.0.1", mqttLogger};
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "HelloWorld\n";
    return 0;
}
