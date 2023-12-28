#pragma once
#include <type_traits>
#include <map>
#include <string>
#include <functional>

#include <iostream>

template <class T>
concept Callable = requires(T foo)
{
    {foo()} -> std::same_as<void>;
};

template <class T, class F, class... Args>
concept CallableOnObject = requires(T* obj, F&& func, Args... args)
{
    {(obj->*func)(std::forward<Args>(args)...)} -> std::same_as<void>;
};

class Signal {
public:
    template<Callable CB>
    bool connect(CB cb){ 
        handlers_.emplace(typeid(cb).name(), [&](){
            cb();
        });
        return true; 
    }
    
    template <class T, class F>
    requires CallableOnObject<T, F>
    bool connect(T* f, F callable)
    {
        handlers_.emplace(typeid(callable).name(), [&](){
            (f->*callable)();
        });
        return true;
    }
    
    bool disconnect();

private:
std::map<std::string, std::function<void(void)>> handlers_;
};