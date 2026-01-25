#include "Core/ThreadPool.h"

using namespace CodeWizard::Core;

ThreadPool::ThreadPool(size_t numThreads) {
    if (numThreads == 0) numThreads = 1;

    for (size_t i = 0; i < numThreads; ++i) {
        m_threads.emplace_back([this] { workerLoop(); });
    }
}

ThreadPool::~ThreadPool() {
    m_stop = true;
    m_condition.notify_all();
    for (auto& thread : m_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPool::workerLoop() {
    while (!m_stop) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_condition.wait(lock, [this] {
                return m_stop || !m_tasks.empty();
            });

            if (m_stop && m_tasks.empty()) {
                return;
            }

            if (!m_tasks.empty()) {
                task = std::move(m_tasks.front());
                m_tasks.pop();
            }
        }

        if (task) {
            task();
        }
    }
}

template<typename F, typename... Args>
auto ThreadPool::submit(F&& f, Args&&... args)
    -> std::future<std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>> {

    using ReturnType = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>;
    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<ReturnType> result = task->get_future();
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        if (!m_stop) {
            m_tasks.emplace([task]() { (*task)(); });
        }
    }
    m_condition.notify_one();
    return result;
}

template<typename F>
void ThreadPool::post(F&& f) {
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        if (!m_stop) {
            m_tasks.emplace(std::forward<F>(f));
        }
    }
    m_condition.notify_one();
}

ThreadPool& globalThreadPool() {
    static ThreadPool pool;
    return pool;
}