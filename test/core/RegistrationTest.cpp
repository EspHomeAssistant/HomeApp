#include <gmock/gmock.h>
#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "core/Registration.hpp"
#include "core/MsgId.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace {
using namespace ::testing;
using nlohmann::json;

class MqttMock : public IMqtt {
public:
    MOCK_METHOD(bool, publish, (const std::string& topic, const std::string& payload));
    MOCK_METHOD(bool, subscribe, (const std::string& topic));
    MOCK_METHOD(std::shared_ptr<spdlog::logger>, getLogger, ());
    MOCK_METHOD((Signal<std::string, std::string>&), onMessage, ());
};

class MqttMessageDispatcherMock : public IMqttMessageDispatcher {
public:
    MOCK_METHOD(void, handle, (const Topic& topic, const std::string& payload));
    MOCK_METHOD(std::shared_ptr<Signal<std::string>>, on,(const Topic&));
};

class RegistrationTest : public Test {
protected:
    RegistrationTest() 
        : mqttMock_{std::make_shared<MqttMock>()}
        , mqttMessageDispatcherMock_{std::make_shared<MqttMessageDispatcherMock>()}
        , signal_{std::make_shared<Signal<std::string>>()}
    {
        EXPECT_CALL(*mqttMock_, subscribe("/register/")).Times(1);
        EXPECT_CALL(*mqttMessageDispatcherMock_, on("/register/")).WillOnce(Return(signal_));

        registration_ = std::make_shared<Registration>(mqttMock_, mqttMessageDispatcherMock_, spdlog::default_logger());
    }


    std::shared_ptr<MqttMock> mqttMock_;
    std::shared_ptr<MqttMessageDispatcherMock> mqttMessageDispatcherMock_;
    std::shared_ptr<Signal<std::string>> signal_;
    std::shared_ptr<Registration> registration_;
};

TEST_F(RegistrationTest, doesNotReactOnMsgWithIdOtherThanRegistrationRequest)
{
    json data;
    data["msgId"] = MsgId::RegistrationResponse;

    EXPECT_CALL(*mqttMock_, publish(_, _)).Times(0);
    registration_->handleRegistration(data.dump());
}

TEST_F(RegistrationTest, publishRegistrationResponseOnSuccess)
{
    json data;
    data["msgId"] = MsgId::RegistrationRequest;
    data["machineId"] = "13:15";

    json publishedData;

    EXPECT_CALL(*mqttMock_, publish("/register/", _)).WillOnce(Invoke([&publishedData](Unused, const std::string& data){
        publishedData = json::parse(data);
        return true;
    }));
    registration_->handleRegistration(data.dump());

    EXPECT_THAT(publishedData["msgId"], Eq(MsgId::RegistrationResponse));
    EXPECT_THAT(publishedData["machineId"], Eq("13:15"));
    EXPECT_THAT(publishedData["status"], Eq("success"));
    EXPECT_THAT(publishedData["registrationId"], Not(Eq(0)));    
}

}