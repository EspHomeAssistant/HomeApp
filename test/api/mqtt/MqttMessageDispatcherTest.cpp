#include <gmock/gmock.h>
#include "api/mqtt/MqttMessageDispatcher.hpp"

namespace {

using namespace ::testing;

struct Foo {
    MOCK_METHOD(void, bar, (const std::string&));
};

class MqttMessageDispatcherTest : public Test {
protected:
    MqttMessageDispatcher dispatcher_;
};

TEST_F(MqttMessageDispatcherTest, createsSignalIfItDoesNotExits)
{
    const std::string topic{"Test"};
    Foo f;
    dispatcher_.on(topic)->connect(&f, &Foo::bar);
}

TEST_F(MqttMessageDispatcherTest, callsHandlerOnHandle)
{
    const std::string topic{"Test"};
    const std::string payload{"TestPayload"};
    Foo f;
    dispatcher_.on(topic)->connect(&f, &Foo::bar);
    EXPECT_CALL(f, bar(payload)).Times(1);
    dispatcher_.handle(topic, payload);
}

}

