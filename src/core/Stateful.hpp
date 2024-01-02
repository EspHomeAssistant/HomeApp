#pragma once

#include <type_traits>
#include "api/Signal.hpp"

template <class T>
concept DefaultConstructible = std::is_default_constructible<T>::value;

template <DefaultConstructible State>
class Stateful {
public:
    using Type = State;
    Signal<State>& onStateChange() {
        return onStateChange_;
    }
    State getState() const {
        return state_;
    }
protected:
    Signal<State> onStateChange_;
    State state_{};
};