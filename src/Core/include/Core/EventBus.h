// include/Core/EventBus.h
#pragma once

#include "Types.h"
#include <functional>
#include <vector>
#include <typeindex>

namespace CodeWizard::Core {

class EventBus {
public:
    template<typename Event>
    using EventHandler = std::function<void(const Event&)>;

    template<typename Event>
    void subscribe(EventHandler<Event> handler) {
        auto& handlers = getHandlers<Event>();
        handlers.push_back(std::move(handler));
    }

    template<typename Event>
    void publish(const Event& event) {
        auto* handlers = tryGetHandlers<Event>();
        if (handlers) {
            for (auto& handler : *handlers) {
                handler(event);
            }
        }
    }

    static EventBus& instance();

private:
    template<typename Event>
    std::vector<EventHandler<Event>>& getHandlers() {
        static std::vector<EventHandler<Event>> handlers;
        return handlers;
    }

    template<typename Event>
    std::vector<EventHandler<Event>>* tryGetHandlers() {
        static std::vector<EventHandler<Event>>* ptr = &getHandlers<Event>();
        return ptr;
    }
};

} // namespace CodeWizard::Core
