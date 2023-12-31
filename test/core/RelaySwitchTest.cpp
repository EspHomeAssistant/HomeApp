#include <gmock/gmock.h>
#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "api/mqtt/MqttMock.hpp"
#include "api/mqtt/MqttMessageDispatcherMock.hpp"
#include "core/RelaySwitch.hpp"
#include "core/MsgId.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace {
using namespace ::testing;
using nlohmann::json;

class RelayStateObserver{
public:
    MOCK_METHOD(void, onRelayState,(std::vector<bool>));
};

class RelaySwitchTest : public Test {
protected:
    RelaySwitchTest() 
        : mqttMock_{std::make_shared<MqttMock>()}
        , mqttMessageDispatcherMock_{std::make_shared<MqttMessageDispatcherMock>()}
        , signal_{std::make_shared<Signal<std::string>>()}
    {
        EXPECT_CALL(*mqttMock_, subscribe(topic_)).Times(1);
        EXPECT_CALL(*mqttMessageDispatcherMock_, on(topic_)).WillOnce(Return(signal_));
        relaySwitch_ = std::make_shared<RelaySwitch>(machineId_, mqttMock_, mqttMessageDispatcherMock_, spdlog::default_logger());
    }

    ~RelaySwitchTest() {
        EXPECT_CALL(*mqttMock_, unsubscribe(topic_)).Times(1);
        EXPECT_CALL(*mqttMessageDispatcherMock_, on(topic_)).WillOnce(Return(signal_));
        relaySwitch_.reset();
    }

    std::shared_ptr<MqttMock> mqttMock_;
    std::shared_ptr<MqttMessageDispatcherMock> mqttMessageDispatcherMock_;
    std::shared_ptr<Signal<std::string>> signal_;
    const std::string machineId_ {"DE:AD:BE:EF"};
    const std::string topic_ {"/sensor/" + machineId_ + "/"};
    std::shared_ptr<RelaySwitch> relaySwitch_;

    const std::vector<bool> relay_{
            false,
            false,
            false,
            true
    };
    const json relayNotif_ {
        {"msgId", MsgId::RelayStateNotif},
        {"state",  relay_},
    };
};

TEST_F(RelaySwitchTest, returnEmptyOnInit)
{
    EXPECT_THAT(relaySwitch_->getRelayState(), ContainerEq(std::vector<bool>{}));
}

TEST_F(RelaySwitchTest, ignoresMessageIfMsgIdIsNotRelayStateNotif)
{
    RelayStateObserver observer;
    relaySwitch_->onStateChange().connect(&observer, &RelayStateObserver::onRelayState);
    EXPECT_CALL(observer, onRelayState(ContainerEq(relay_))).Times(0);

    json wrongMsgId = relayNotif_;
    wrongMsgId["msgId"] = MsgId::TemperatureNotif;

    signal_->emit(wrongMsgId.dump());
}

TEST_F(RelaySwitchTest, updatesRelayState)
{
    signal_->emit(relayNotif_.dump());
    EXPECT_THAT(relaySwitch_->getRelayState(), ContainerEq(relay_));
}

TEST_F(RelaySwitchTest, emitsRelayStateOnUpdate)
{
    RelayStateObserver observer;
    relaySwitch_->onStateChange().connect(&observer, &RelayStateObserver::onRelayState);
    EXPECT_CALL(observer, onRelayState(ContainerEq(relay_))).Times(1);

    signal_->emit(relayNotif_.dump());
}

TEST_F(RelaySwitchTest, onSendsAllRelayOutputsAsTrue)
{
    using nlohmann::json;
    const json expected {
        {"msgId", MsgId::RelayStateRequest},
        {"state", std::vector<bool>{true, true, true,true}}
    }; 
    signal_->emit(relayNotif_.dump());
    EXPECT_CALL(*mqttMock_, publish(topic_, StrEq(expected.dump()))).Times(1);
    relaySwitch_->on();
}

TEST_F(RelaySwitchTest, offSendsAllRelayOutputsAsFalse)
{
    using nlohmann::json;
    const json expected {
        {"msgId", MsgId::RelayStateRequest},
        {"state", std::vector<bool>{false, false, false, false}}
    }; 
    signal_->emit(relayNotif_.dump());
    EXPECT_CALL(*mqttMock_, publish(topic_, StrEq(expected.dump()))).Times(1);
    relaySwitch_->off();
}


}