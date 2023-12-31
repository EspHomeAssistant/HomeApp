#include <gmock/gmock.h>
#include "core/RegisteredDevice.hpp"

namespace {

using namespace ::testing;

struct Param {
    std::string input;
    DeviceType expectedType;
};

class RegisteredDeviceTest : public ::testing::TestWithParam<Param> {
protected:
    const std::string machineId_ {"DE:AD:BE:EF"};
};

TEST_P(RegisteredDeviceTest, produceDeviceIfCorrectTypeIsPassed)
{
    const auto param = GetParam();
    RegisteredDevice device{machineId_, param.input};
    EXPECT_THAT(device.machineId, StrEq(machineId_));
    EXPECT_THAT(device.type, Eq(param.expectedType));
}

const std::vector<Param> params {
    {"temperature", DeviceType::Temperature},
    {"TEMPERATURE", DeviceType::Temperature},
    {"TeMpErAtUrE", DeviceType::Temperature},
    {"relay", DeviceType::Relay},
    {"RELAY", DeviceType::Relay},
    {"ReLaY", DeviceType::Relay},
    {"Unknown", DeviceType::Unknown},
    {"", DeviceType::Unknown},
    {"SomeOtherTypeWhichIsNotKnown", DeviceType::Unknown},
};

INSTANTIATE_TEST_SUITE_P(, RegisteredDeviceTest, ValuesIn(params), [](const testing::TestParamInfo<Param>& info){
    return (info.param.input.empty() ? "empty" : info.param.input);
});

}