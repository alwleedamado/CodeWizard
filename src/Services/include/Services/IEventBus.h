#pragma once

#include <functional>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace CodeWizard::Services {

/**
 * @brief Subscriber handle, used to unsubscribe.
 */
class ISubscriberHandle
{
public:
    virtual ~ISubscriberHandle() = default;

    /**
     * @brief Unsubscribe from the event.
     */
    virtual void unsubscribe() = 0;
};

/**
 * @brief Simple, high-performance event bus interface.
 * Supports multiple subscribers and decoupled communication.
 */
class IEventBus
{
public:
    virtual ~IEventBus() = default;

    /**
     * @brief Subscribe to an event by name.
     * @param eventName Event identifier.
     * @param callback Function invoked when event is published.
     * @return Handle to manage subscription.
     */
    virtual std::shared_ptr<ISubscriberHandle> subscribe(
        const std::string& eventName,
        std::function<void(void*)> callback
    ) = 0;

    /**
     * @brief Publish an event to all subscribers.
     * Should be non-blocking or internally asynchronous.
     */
    virtual void publish(const std::string& eventName, void* payload) = 0;
};

} // namespace CodeWizard::Services
