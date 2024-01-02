#include <gmock/gmock.h>

#include <vector>
#include <spdlog/spdlog.h>

#include "core/RegisteredDevice.hpp"
#include "core/DeviceController.hpp"
#include "core/Termometer.hpp"
#include "api/mqtt/MqttMock.hpp"
#include "api/mqtt/MqttMessageDispatcherMock.hpp"
#include "core/RegistrationMock.hpp"


namespace {
using namespace ::testing;

class DeviceControllerTest : public Test {
protected:
    DeviceControllerTest()
     : mqtt_{std::make_shared<MqttMock>()}
     , mqttMessageDispatcher_{std::make_shared<MqttMessageDispatcherMock>()}
     , monitoringSignal_{std::make_shared<Signal<std::string>>()}
     , registration_{std::make_shared<RegistrationMock>()}
     {
        EXPECT_CALL(*registration_, onRegistration()).WillOnce(ReturnRef(signal_));
        control_ = std::make_shared<DeviceController>(registration_, mqtt_, mqttMessageDispatcher_, spdlog::default_logger());
     }
    ~DeviceControllerTest() {
        EXPECT_CALL(*registration_, onRegistration()).WillOnce(ReturnRef(signal_));
        control_.reset();
    }
    std::shared_ptr<MqttMock> mqtt_;
    std::shared_ptr<MqttMessageDispatcherMock> mqttMessageDispatcher_;
    Signal<RegisteredDevice> signal_;
    //@todo how to use a MqttEntityMock
    std::shared_ptr<Signal<std::string>> monitoringSignal_;
    std::shared_ptr<RegistrationMock> registration_;
    std::shared_ptr<DeviceController> control_;

    const std::string machineId_ {"DE:AD:BE:EF"};
};

TEST_F(DeviceControllerTest, returnsNullPtrIfNoDeviceWithGivenIdHasBeenRegistered)
{
    EXPECT_THAT(control_->getDevice(machineId_), Eq(nullptr));
}

TEST_F(DeviceControllerTest, storeDeviceIfItsGetRegistered) {
    const RegisteredDevice device {machineId_, "temperature"};
    //@todo how to use a TemperatureMock here? How to construct the TemperatureMock??
    EXPECT_CALL(*mqtt_, subscribe(_)).Times(1);
    EXPECT_CALL(*mqtt_, unsubscribe(_)).Times(1);
    EXPECT_CALL(*mqttMessageDispatcher_, on(_)).Times(2).WillRepeatedly(Return(monitoringSignal_));
    signal_.emit(device);

    ASSERT_THAT(control_->getDevice(machineId_), Not(Eq(nullptr)));
    EXPECT_THAT(control_->getDevice(machineId_)->getMachineId(), StrEq(machineId_));
    EXPECT_THAT(control_->getDevice(machineId_)->getDeviceType(), Eq(DeviceType::Temperature));
}

}