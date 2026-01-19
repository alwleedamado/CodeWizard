// src/Timer.cpp
#include "Platform/Timer.h"
#include <Core/Task.h>
#include <atomic>
#include <thread>
#include <condition_variable>

namespace CodeWizard::Platform {

struct Timer::Impl {
    Core::TaskScheduler::Duration interval;
    Core::TaskScheduler::Task callback;
    std::atomic<bool> active{false};
    std::thread worker;
    std::mutex mutex;
    std::condition_variable cv;

    Impl(Core::TaskScheduler::Duration intv, Core::TaskScheduler::Task cb)
        : interval(intv), callback(std::move(cb)) {}

    ~Impl() {
        stop();
        if (worker.joinable()) {
            worker.join();
        }
    }

    void start() {
        std::lock_guard lock(mutex);
        if (active) return;
        active = true;
        worker = std::thread([this]() {
            while (true) {
                std::unique_lock<std::mutex> lock(mutex);
                if (!active) break;
                if (cv.wait_for(lock, interval) == std::cv_status::timeout) {
                    if (active) {
                        lock.unlock();
                        try {
                            callback();
                        } catch (...) {
                            // Ignore errors
                        }
                    }
                } else {
                    break; // Stop requested
                }
            }
        });
    }

    void stop() {
        std::lock_guard lock(mutex);
        if (!active) return;
        active = false;
        cv.notify_all();
    }
};

Timer::Timer(Duration interval, Callback callback)
    : m_impl(std::make_unique<Impl>(interval, [cb = std::move(callback)]() { cb(); })) {}

Timer::~Timer() = default;

void Timer::start() {
    m_impl->start();
}

void Timer::stop() {
    m_impl->stop();
}

bool Timer::isActive() const noexcept {
    return m_impl->active.load();
}

void Timer::setInterval(Duration interval) {
    m_impl->interval = interval;
}

Timer::Duration Timer::interval() const noexcept {
    return m_impl->interval;
}

} // namespace CodeWizard::Platform
