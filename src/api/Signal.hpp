#pragma once
#include <type_traits>
#include <map>
#include <string>
#include <functional>
#include <memory>

template <class T, class... Args>
concept Callable = requires(T foo, Args... args)
{
    {foo(std::forward<Args>(args)...)};
    {foo(std::forward<Args>(args)...)} -> std::same_as<void>;
};

template <class T, class F, class... Args>
concept CallableOnObject = requires(T* obj, F&& func, Args... args)
{
    {(obj->*func)(std::forward<Args>(args)...)} -> std::same_as<void>;
};

template<class... Args>
class Signal {
public:
    template<class F>
    requires Callable<F, Args...>
    void connect(F callable)
    //Should I use perfect forwarding or shared_ptr to manage 'callable' lifetime?
    //I am not sure why this helped here, where the dangling reference were produced?
    //Was the lambda object gone out of the scope? Why shared_ptr is needed here if I've emplaced an lambda into a map?
    
    { 
        auto handler = std::make_shared<std::function<void(Args...)>>([callable](Args... args){
            std::invoke(callable, args...);
        });
        
        handlers_.emplace(typeid(callable).name(), handler);
    }
    
    template <class T, class F>
    requires CallableOnObject<T, F, Args...>
    void connect(T* obj, F callable)
    {
        auto handler = std::make_shared<std::function<void(Args...)>>([obj, callable](Args... args){
            std::invoke(callable, obj, args...);
        });

        const std::string key {std::to_string(reinterpret_cast<long>(obj)) + typeid(callable).name()};

        handlers_.emplace(key, handler);
    }
    
    void emit(Args... args) {
        for (const auto&[_, handler] : handlers_) {
            std::invoke(*handler, args...);
        }
    }

    template <class T, class F>
    requires CallableOnObject<T, F, Args...>
    void disconnect(T* obj, F callable) {
        const std::string key {std::to_string(reinterpret_cast<long>(obj)) + typeid(callable).name()};
        handlers_.erase(key);
    }

    void disconnect(){
        handlers_ = std::map<std::string, std::shared_ptr<std::function<void(Args...)>>>{};
    }

private:
    std::map<std::string, std::shared_ptr<std::function<void(Args...)>>> handlers_;
};