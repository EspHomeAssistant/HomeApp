#include <gmock/gmock.h>
#include "api/mqtt/Mqtt.hpp"
#include "api/mqtt/MqttMessageDispatcher.hpp"
#include "api/mqtt/MqttMock.hpp"
#include "api/mqtt/MqttMessageDispatcherMock.hpp"
#include "core/Registration.hpp"
#include "core/MsgId.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace {
using namespace ::testing;
using nlohmann::json;

class RegistrationObserver {
public:
    MOCK_METHOD(void, onRegistration, (const RegisteredDevice&));
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
    const json data {
        {"machineId", "DE:AD:BE:EF"},
        {"msgId", MsgId::RegistrationRequest},
        {"sensorType", "temperature"},
    };

    json publishedData;

    EXPECT_CALL(*mqttMock_, publish("/register/", _)).WillOnce(Invoke([&publishedData](Unused, const std::string& data){
        publishedData = json::parse(data);
        return true;
    }));
    registration_->handleRegistration(data.dump());

    EXPECT_THAT(publishedData["msgId"], Eq(MsgId::RegistrationResponse));
    EXPECT_THAT(publishedData["machineId"], StrEq("DE:AD:BE:EF"));
    EXPECT_THAT(publishedData["status"], Eq("success"));
    EXPECT_THAT(publishedData["registrationId"], Not(Eq(0)));
}

TEST_F(RegistrationTest, returnsDifferentRegistrationIdsForDifferentMachines)
{
    const json firstData {
        {"machineId", "DE:AD:BE:EF"},
        {"msgId", MsgId::RegistrationRequest},
        {"sensorType", "temperature"},
    };
    const json secondData {
        {"machineId", "DE:AD:BE:EE"},
        {"msgId", MsgId::RegistrationRequest},
        {"sensorType", "temperature"},
    };

    int firstRegistrationId{-1};
    int secondRegistrationId{-1};

    EXPECT_CALL(*mqttMock_, publish("/register/", _)).WillOnce(Invoke([&firstRegistrationId](Unused, const std::string& data){
        firstRegistrationId = json::parse(data)["registrationId"];
        return true;
    })).WillOnce(Invoke([&secondRegistrationId](Unused, const std::string& data){
        secondRegistrationId = json::parse(data)["registrationId"];
        return true;
    }));

    registration_->handleRegistration(firstData.dump());
    registration_->handleRegistration(secondData.dump());

    EXPECT_THAT(firstRegistrationId, Not(Eq(secondRegistrationId)));
}

TEST_F(RegistrationTest, returnsSameRegistrationIdForTheSameMachine)
{
    const json data {
        {"machineId", "DE:AD:BE:EF"},
        {"msgId", MsgId::RegistrationRequest},
        {"sensorType", "temperature"},
    };
    int firstRegistrationId{-1};
    int secondRegistrationId{-1};

    EXPECT_CALL(*mqttMock_, publish("/register/", _)).WillOnce(Invoke([&firstRegistrationId](Unused, const std::string& data){
        firstRegistrationId = json::parse(data)["registrationId"];
        return true;
    })).WillOnce(Invoke([&secondRegistrationId](Unused, const std::string& data){
        secondRegistrationId = json::parse(data)["registrationId"];
        return true;
    }));

    registration_->handleRegistration(data.dump());
    registration_->handleRegistration(data.dump());

    EXPECT_THAT(firstRegistrationId, Eq(secondRegistrationId));
}

TEST_F(RegistrationTest, emitsRegisteredDeviceOnFirstRegistration)
{
    const json data {
        {"machineId", "DE:AD:BE:EF"},
        {"msgId", MsgId::RegistrationRequest},
        {"sensorType", "temperature"},
    };
    const RegisteredDevice expectedDevice {"DE:AD:BE:EF", "temperature"};
    RegistrationObserver observer;
    registration_->onRegistration().connect(&observer, &RegistrationObserver::onRegistration);
    EXPECT_CALL(observer, onRegistration(Eq(expectedDevice))).Times(1);
    EXPECT_CALL(*mqttMock_, publish("/register/", _)).Times(2);

    registration_->handleRegistration(data.dump());
    registration_->handleRegistration(data.dump());   
}

}