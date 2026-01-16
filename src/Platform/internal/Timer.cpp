#include "Timer.h"
#include <chrono>
#include <thread>
namespace CodeWizard::Platform::Internal {
void Timer::start(int intervalMs, std::function<void()> callback) {
    stop();
    m_running = true;
    m_thread = std::thread([=]() {
        while (m_running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
            if (m_running) callback();
        }
    });
}
void Timer::stop() {
    m_running = false;
    if (m_thread.joinable()) m_thread.join();
}
}
