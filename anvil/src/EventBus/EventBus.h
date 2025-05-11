#pragma once

#include "../Logger/Logger.h"
#include "Event.h"
#include <map>
#include <typeindex>
#include <functional>
#include <list>

namespace anvil {

class IEventCallback {
private:
    virtual void Call(Event& e) = 0;

public:
    virtual ~IEventCallback() = default;
        
    void Execute(Event& e) {
        Call(e);
    }
};

template<typename TEvent>
using CallbackFunction = std::function<void(TEvent&)>;

template<typename TEvent>
class EventCallback : public IEventCallback {
private:
    CallbackFunction<TEvent> callback;

public:
    explicit EventCallback(CallbackFunction<TEvent> cf)
        : callback(cf) {}

    void Call(Event& e) final {
        callback(static_cast<TEvent&>(e));
    }
};

class EventBus {
    using CallbackList = std::vector<std::unique_ptr<IEventCallback>>;
    std::map<std::type_index, CallbackList> subscribers{};

public:
    EventBus() = default;
    virtual ~EventBus() = default;

    void Reset() {
        subscribers.clear();
    }

    template<typename TEvent>
    void SubscribeToEvent(CallbackFunction<TEvent> cf) {
        auto key = std::type_index(typeid(TEvent));
        if (!subscribers.contains(key)) {
            subscribers[key] = CallbackList();
        }
        auto sub = std::make_unique<EventCallback<TEvent>>(cf);
        subscribers[key].push_back(std::move(sub));
    }

    template<typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackMethod)(TEvent&)) {
        auto bound = [ownerInstance, callbackMethod](TEvent& event) {
            (ownerInstance->*callbackMethod)(event);
        };
        SubscribeToEvent<TEvent>(bound);
    }

    template<typename TEvent, typename... TArgs>
    void EmitEvent(TArgs&&... args) {
        auto key = std::type_index(typeid(TEvent));

        if (auto found = subscribers.find(key); found != subscribers.end()) {
            for (auto& iEventCB: found->second) {
                TEvent ev(std::forward<TArgs>(args)...);
                Logger::Log("eventBus::EmitEvent({})", key.name());
                iEventCB->Execute(ev);
            }
        }
    }
};
}