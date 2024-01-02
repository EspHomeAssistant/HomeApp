#include <gmock/gmock.h>

#include "core/Stateful.hpp"

namespace {
using namespace ::testing;

class SignalObserver {
public:
    MOCK_METHOD(void, onStateChanged, (double));
};

class StatefulTest : public Test {
protected:
    Stateful<double> stateful_;
};

TEST_F(StatefulTest, returnsADefaultStateOnInit) {
    EXPECT_THAT(stateful_.getState(), Eq(double{}));
}

TEST_F(StatefulTest, returnsSignalToConnectTo) {
    const double val {13.15};
    SignalObserver observer;
    EXPECT_CALL(observer, onStateChanged(DoubleEq(val))).Times(1);
    
    stateful_.onStateChange().connect(&observer, &SignalObserver::onStateChanged);

    stateful_.onStateChange().emit(val);
}

}