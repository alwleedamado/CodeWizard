#pragma once
#include "Platform/ITimer.h"
#include <atomic>
#include <thread>
namespace CodeWizard::Platform::Internal {
class Timer final : public ITimer {
public:
    void start(int intervalMs, std::function<void()> callback) override;
    void stop() override;
private:
    std::atomic<bool> m_running{false};
    std::thread m_thread;
};
}
