#pragma once

#include <functional>
#include <future>
#include <memory>

namespace CodeWizard::Services {

/**
 * @brief Represents a handle to a scheduled task.
 * Allows cancellation or waiting for completion.
 */
class ITaskHandle
{
public:
    virtual ~ITaskHandle() = default;

    /**
     * @brief Cancel the task if not yet running.
     * @return True if successfully canceled.
     */
    virtual bool cancel() = 0;

    /**
     * @brief Wait for task completion.
     */
    virtual void wait() = 0;

    /**
     * @brief Check if the task has completed.
     */
    virtual bool isDone() const = 0;
};

/**
 * @brief Generic async task scheduler interface.
 * Tasks run in background threads managed by the scheduler.
 */
class ITaskScheduler
{
public:
    virtual ~ITaskScheduler() = default;

    /**
     * @brief Schedule a task to run asynchronously.
     * @param task Function to execute in the background.
     * @return Handle to manage the task.
     */
    virtual std::shared_ptr<ITaskHandle> schedule(std::function<void()> task) = 0;

    /**
     * @brief Schedule a task with a delayed start (in milliseconds).
     */
    virtual std::shared_ptr<ITaskHandle> scheduleDelayed(std::function<void()> task, int delayMs) = 0;

    /**
     * @brief Wait until all scheduled tasks are finished.
     */
    virtual void waitAll() = 0;
};

} // namespace CodeWizard::Services
