#include <gmock/gmock.h>
#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "api/mqtt/MqttMock.hpp"
#include "api/mqtt/MqttMessageDispatcherMock.hpp"
#include "core/MqttEntity.hpp"
#include "core/DeviceType.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace {
using namespace ::testing;
using nlohmann::json;

class Concrete : public MqttEntity<double>{
public:
    Concrete(const std::string& machineId,
            std::shared_ptr<IMqtt> mqtt,
            std::shared_ptr<IMqttMessageDispatcher> mqttMessageDispatcher,
            std::shared_ptr<spdlog::logger> logger)
            : MqttEntity{machineId, mqtt, mqttMessageDispatcher, logger}
            {}
    Type getState() const {
        return MqttEntity::getState();
    }
    DeviceType getDeviceType() const override {
        return DeviceType::Temperature;
    }
    MOCK_METHOD(void, handleMessage, (const std::string& payload), (override));
};

class MqttEntityTest : public Test {
protected:
    MqttEntityTest() 
        : mqttMock_{std::make_shared<MqttMock>()}
        , mqttMessageDispatcherMock_{std::make_shared<MqttMessageDispatcherMock>()}
        , signal_{std::make_shared<Signal<std::string>>()}
    {
        EXPECT_CALL(*mqttMock_, subscribe(topic_)).Times(1);
        EXPECT_CALL(*mqttMessageDispatcherMock_, on(topic_)).WillOnce(Return(signal_));
        mqttEntity_ = std::make_shared<Concrete>(machineId_, mqttMock_, mqttMessageDispatcherMock_, spdlog::default_logger());
    }

    ~MqttEntityTest() {
        EXPECT_CALL(*mqttMock_, unsubscribe(topic_)).Times(1);
        EXPECT_CALL(*mqttMessageDispatcherMock_, on(topic_)).WillOnce(Return(signal_));
        mqttEntity_.reset();
    }

    std::shared_ptr<MqttMock> mqttMock_;
    std::shared_ptr<MqttMessageDispatcherMock> mqttMessageDispatcherMock_;
    std::shared_ptr<Signal<std::string>> signal_;
    const std::string machineId_ {"DE:AD:BE:EF"};
    const std::string topic_ {"/sensor/" + machineId_ + "/"};
    std::shared_ptr<Concrete> mqttEntity_;
};

TEST_F(MqttEntityTest, returnDefaultStateOnInit)
{
    EXPECT_THAT(mqttEntity_->getState(), Eq(Concrete::Type{}));
}

TEST_F(MqttEntityTest, handlesMessageOnMsgArrive)
{
    const std::string data{"some data"};
    EXPECT_CALL(*mqttEntity_, handleMessage(StrEq(data))).Times(1);
    signal_->emit(data);
}

TEST_F(MqttEntityTest, returnsMachineId)
{
    EXPECT_THAT(mqttEntity_->getMachineId(), StrEq(machineId_));    
}

TEST_F(MqttEntityTest, returnsDeviceType)
{
    EXPECT_THAT(mqttEntity_->getDeviceType(), Eq(DeviceType::Temperature));
}

}