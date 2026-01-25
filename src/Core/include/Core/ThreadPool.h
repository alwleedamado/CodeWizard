#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace CodeWizard::Core {

class ThreadPool {
public:
  explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
  ~ThreadPool();

  // Submit task - returns future for result
  template<typename F, typename... Args>
  auto submit(F&& f, Args&&... args)
      -> std::future<std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>;

  // Submit fire-and-forget task
  template<typename F>
  void post(F&& f);

  [[nodiscard]] size_t size() const { return m_threads.size(); }

private:
  void workerLoop();

  std::vector<std::thread> m_threads;
  std::queue<std::function<void()>> m_tasks;
  std::mutex m_queueMutex;
  std::condition_variable m_condition;
  std::atomic<bool> m_stop{false};
};

// Global access
ThreadPool& globalThreadPool();

} // namespace CodeWizard::Core