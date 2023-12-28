#include "api/Signal.hpp"
#include <gmock/gmock.h>

#include <iostream>

namespace {
using namespace ::testing;

struct Foo {
    MOCK_METHOD(void, bar, (int));
    MOCK_METHOD(void, baz, (int), (const));
};

class SignalTest : public Test{
protected:   
    Signal<int> signal_;
};

void foo(int) {}

TEST_F(SignalTest, canConnectFreeFunction)
{
    signal_.connect(foo);
}

TEST_F(SignalTest, canConnectLambda)
{
    signal_.connect([](int){});
}

TEST_F(SignalTest, canConnectMemberFunction)
{
    Foo f;
    signal_.connect(&f, &Foo::bar);
}
//how about TypedTest?

TEST_F(SignalTest, handlerShouldBeCalledOnEmit)
{
    Foo f;
    signal_.connect(&f, &Foo::bar);
    EXPECT_CALL(f, bar(_)).Times(1);
    signal_.emit(13);
}

TEST_F(SignalTest, disconnectDisconnectsHandler)
{   
    Foo f;
    signal_.connect(&f, &Foo::bar);
    signal_.disconnect();
    EXPECT_CALL(f, bar(_)).Times(0);
    signal_.emit(13);
}

TEST_F(SignalTest, disconnectWithAFunctionGivenToConnectDisconnectsOnlyThisSpecificHandler)
{   
    Foo f;
    signal_.connect(&f, &Foo::bar);
    signal_.disconnect(&f, &Foo::bar);
    EXPECT_CALL(f, bar(_)).Times(0);
    signal_.emit(13);
}

TEST_F(SignalTest, canConnectMoreThanOneInstanceOfAClass)
{   
    Foo f;
    Foo g;
    signal_.connect(&f, &Foo::bar);
    signal_.connect(&g, &Foo::bar);
    EXPECT_CALL(f, bar(_)).Times(1);
    EXPECT_CALL(g, bar(_)).Times(1);
    signal_.emit(13);
}

TEST_F(SignalTest, canDisconnectOneInstanceOfAClass)
{   
    Foo f;
    Foo g;
    signal_.connect(&f, &Foo::bar);
    signal_.connect(&g, &Foo::bar);
    signal_.disconnect(&g, &Foo::bar);
    EXPECT_CALL(f, bar(_)).Times(1);
    EXPECT_CALL(g, bar(_)).Times(0);
    signal_.emit(13);
}

TEST_F(SignalTest, canConnectConstMemberFunction)
{   
    Foo f;
    signal_.connect(&f, &Foo::baz);
    EXPECT_CALL(f, baz(_)).Times(1);
    signal_.emit(13);
}

}
