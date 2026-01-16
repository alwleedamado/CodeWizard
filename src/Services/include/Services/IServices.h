#pragma once

#include <functional>

namespace CodeWizard::Services {

/**
 * @brief Task scheduling interface.
 */
class ITaskScheduler
{
public:
    virtual ~ITaskScheduler() = default;

    /**
     * @brief Schedule a task to run asynchronously.
     */
    virtual void schedule(std::function<void()> task) = 0;
};

/**
 * @brief Event bus interface.
 */
class IEventBus
{
public:
    virtual ~IEventBus() = default;

    /**
     * @brief Publish an event to subscribers.
     */
    virtual void publish(const std::string& eventName, void* payload) = 0;
};

} // namespace CodeWizard::Services
