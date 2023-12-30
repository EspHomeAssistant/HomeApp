#include <gmock/gmock.h>
#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "api/mqtt/MqttMock.hpp"
#include "api/mqtt/MqttMessageDispatcherMock.hpp"
#include "core/Termometer.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace {
using namespace ::testing;
using nlohmann::json;

class TemperatureObserver{
public:
    MOCK_METHOD(void, onTemperature,(double));
};

class TermometerTest : public Test {
protected:
    TermometerTest() 
        : mqttMock_{std::make_shared<MqttMock>()}
        , mqttMessageDispatcherMock_{std::make_shared<MqttMessageDispatcherMock>()}
        , signal_{std::make_shared<Signal<std::string>>()}
    {
        EXPECT_CALL(*mqttMock_, subscribe(topic_)).Times(1);
        EXPECT_CALL(*mqttMessageDispatcherMock_, on(topic_)).WillOnce(Return(signal_));
        termometer_ = std::make_shared<Termometer>(machineId_, mqttMock_, mqttMessageDispatcherMock_, spdlog::default_logger());
    }

    ~TermometerTest() {
        EXPECT_CALL(*mqttMock_, unsubscribe(topic_)).Times(1);
        EXPECT_CALL(*mqttMessageDispatcherMock_, on(topic_)).WillOnce(Return(signal_));
        termometer_.reset();
    }

    std::shared_ptr<MqttMock> mqttMock_;
    std::shared_ptr<MqttMessageDispatcherMock> mqttMessageDispatcherMock_;
    std::shared_ptr<Signal<std::string>> signal_;
    const std::string machineId_ {"DE:AD:BE:EF"};
    const std::string topic_ {"/sensor/" + machineId_ + "/"};
    std::shared_ptr<Termometer> termometer_;

    const double temperature_ {36.6f};
    const json data_ {
        {"temperature", temperature_ },
    };
};

TEST_F(TermometerTest, returnZeroTemperatureOnInit)
{
    EXPECT_THAT(termometer_->getTemperature(), DoubleEq(0));
}

TEST_F(TermometerTest, updatesTemperature)
{
    signal_->emit(data_.dump());
    EXPECT_THAT(termometer_->getTemperature(), DoubleEq(temperature_));
}

TEST_F(TermometerTest, emitsTemperatureOnUpdate)
{
    TemperatureObserver observer;
    termometer_->onTemperatureChange().connect(&observer, &TemperatureObserver::onTemperature);
    EXPECT_CALL(observer, onTemperature(DoubleEq(temperature_))).Times(1);

    signal_->emit(data_.dump());
}


}