#pragma once
#include <functional>
namespace CodeWizard::Services {
class ITaskScheduler {
public:
    virtual ~ITaskScheduler() = default;
    virtual void postTask(std::function<void()> task) = 0;
};
}
