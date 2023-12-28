#include "api/Signal.hpp"
#include <gmock/gmock.h>

namespace {
using namespace ::testing;

void foo() {}

struct Foo {
void bar() {}
};

class SignalTest : public Test{
public: 
    void bar() {}

protected:   
    Signal signal_;
};

TEST_F(SignalTest, canConnectFreeFunction)
{
    signal_.connect(foo);
}

TEST_F(SignalTest, canConnectLambda)
{
    signal_.connect([](){});
}

TEST_F(SignalTest, canConnectMemberFunction)
{
    Foo f;
    signal_.connect(&f, &Foo::bar);
}

}
