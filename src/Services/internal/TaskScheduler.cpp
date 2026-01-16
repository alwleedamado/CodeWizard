#include "TaskScheduler.h"
namespace CodeWizard::Services::Internal {
void TaskScheduler::postTask(std::function<void()> task) {
    m_tasks.push(std::move(task));
    // Note: Actual async execution will be implemented later
}
}
