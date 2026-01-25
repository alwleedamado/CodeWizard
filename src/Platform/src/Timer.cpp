#include "Platform/Timer.h"
#include "Core/ThreadPool.h"
#include <atomic>
#include <thread>
#include <condition_variable>

using namespace CodeWizard::Core;

struct Timer::Impl {
    std::function<void()> callback;
    Duration delay;
    bool repeating = false;
    std::atomic<bool> active{false};
    std::atomic<bool> shouldStop{false};
    std::thread workerThread;
    std::mutex mutex;
    std::condition_variable cv;
    std::chrono::milliseconds repeatIntervalMs{0};

    ~Impl() {
        stop();
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

    void start() {
        if (active.exchange(true)) return; // Already running

        shouldStop.store(false);
        workerThread = std::thread([this]() {
            run();
        });
    }

    void stop() {
        if (!active.exchange(false)) return; // Not running

        shouldStop.store(true);
        cv.notify_all();
    }

    void run() {
        while (active.load() && !shouldStop.load()) {
            // Wait for the specified delay
            {
                std::unique_lock<std::mutex> lock(mutex);
                if (cv.wait_for(lock, delay, [this]() { return shouldStop.load(); })) {
                    // Was notified (stop requested)
                    break;
                }
            }

            // Check if we should still execute
            if (!active.load() || shouldStop.load()) {
                break;
            }

            // Execute callback in thread pool (non-blocking)
            auto callbackCopy = callback;
            globalThreadPool().post([callbackCopy = std::move(callbackCopy)]() {
                try {
                    callbackCopy();
                } catch (...) {
                    // Log error or handle as needed
                }
            });

            // If not repeating, exit after first execution
            if (!repeating) {
                active.store(false);
                break;
            }

            // For repeating timers, use repeatInterval
            if (repeatIntervalMs.count() > 0) {
                delay = repeatIntervalMs;
            }
        }

        active.store(false);
    }

    void setRepeatInterval(std::chrono::milliseconds interval) {
        repeatIntervalMs = interval;
    }

    std::chrono::milliseconds getRepeatInterval() const {
        return repeatIntervalMs;
    }
};

// Constructor
Timer::Timer(Duration delay, std::function<void()> callback, bool repeating)
    : m_impl(std::make_unique<Impl>()) {
    m_impl->delay = delay;
    m_impl->callback = std::move(callback);
    m_impl->repeating = repeating;
}

// Template methods
template<typename Rep, typename Period>
std::unique_ptr<Timer> Timer::singleShot(
    const std::chrono::duration<Rep, Period>& delay,
    std::function<void()> callback) {

    auto timer = std::make_unique<Timer>(
        std::chrono::duration_cast<Duration>(delay),
        std::move(callback),
        false
    );
    timer->start();
    return timer;
}

template<typename Rep, typename Period>
std::unique_ptr<Timer> Timer::repeat(
    const std::chrono::duration<Rep, Period>& interval,
    std::function<void()> callback) {

    auto timer = std::make_unique<Timer>(
        std::chrono::duration_cast<Duration>(interval),
        std::move(callback),
        true
    );
    timer->start();
    return timer;
}

// Explicit instantiations for common durations
template std::unique_ptr<Timer> Timer::singleShot<std::chrono::milliseconds::rep, std::milli>(
    const std::chrono::duration<std::chrono::milliseconds::rep, std::milli>&,
    std::function<void()>
);

template std::unique_ptr<Timer> Timer::repeat<std::chrono::milliseconds::rep, std::milli>(
    const std::chrono::duration<std::chrono::milliseconds::rep, std::milli>&,
    std::function<void()>
);

template std::unique_ptr<Timer> Timer::singleShot<std::chrono::seconds::rep, std::ratio<1>>(
    const std::chrono::duration<std::chrono::seconds::rep, std::ratio<1>>&,
    std::function<void()>
);

template std::unique_ptr<Timer> Timer::repeat<std::chrono::seconds::rep, std::ratio<1>>(
    const std::chrono::duration<std::chrono::seconds::rep, std::ratio<1>>&,
    std::function<void()>
);

// Destructor
Timer::~Timer() = default;

// Control methods
void Timer::start() { m_impl->start(); }
void Timer::stop() { m_impl->stop(); }
bool Timer::isActive() const { return m_impl->active.load(); }

void Timer::setRepeatInterval(std::chrono::milliseconds interval) {
    m_impl->setRepeatInterval(interval);
}

std::chrono::milliseconds Timer::repeatInterval() const {
    return m_impl->getRepeatInterval();
}