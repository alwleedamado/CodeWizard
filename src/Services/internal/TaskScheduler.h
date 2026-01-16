#pragma once
#include "Services/ITaskScheduler.h"
#include <queue>
#include <functional>
namespace CodeWizard::Services::Internal {
class TaskScheduler final : public ITaskScheduler {
public:
    void postTask(std::function<void()> task) override;
private:
    std::queue<std::function<void()>> m_tasks;
};
}
