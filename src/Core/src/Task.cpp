// src/Task.cpp
#include "Core/Task.h"
#include <queue>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <cassert>

namespace CodeWizard::Core {

TaskScheduler& TaskScheduler::instance() {
    static TaskScheduler inst;
    return inst;
}

TaskScheduler::TaskScheduler() : m_impl(std::make_unique<Impl>()) {}

TaskId TaskScheduler::schedule(Task task, Duration delay) {
    return m_impl->schedule(std::move(task), delay);
}

void TaskScheduler::cancel(TaskId id) {
    m_impl->cancel(id);
}

} // namespace CodeWizard::Core
