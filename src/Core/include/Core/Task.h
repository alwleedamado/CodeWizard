// include/Core/Task.h
#pragma once

#include "Types.h"
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <queue>
#ifdef __cpp_lib_jthread
    #include <stop_token>
#include <thread>
using jthread = std::jthread;
#else
#include <thread>
struct jthread {
  std::thread t;
 std::atomic<bool> stop_requested{false};
  jthread(std::function<void()> f) : t([this, f = std::move(f)] { f(); }) {}
  ~jthread() { if (t.joinable()) t.join(); }
  bool request_stop() { return stop_requested.exchange(true); }
 bool stop_requested() const { return stop_requested.load(); }
};
#endif

namespace CodeWizard::Core {

using TaskId = Id;

class TaskScheduler
{
public:
  using Task = std::function<void()>;
  using TimePoint = std::chrono::steady_clock::time_point;
  using Duration = std::chrono::milliseconds;

  struct Impl
  {
    struct ScheduledTask
    {
      TaskId id;
      TimePoint executeAt;
      Task task;

      bool operator>(const ScheduledTask &other) const { return executeAt > other.executeAt; }
    };

    std::priority_queue<ScheduledTask, std::vector<ScheduledTask>, std::greater<ScheduledTask>> tasks;
    std::unordered_map<TaskId, bool> cancelled;
    std::mutex mutex;
    std::condition_variable cv;
    jthread worker;
    std::atomic<TaskId> nextId{ 1 };
    std::atomic<bool> shutdown{ false };

    Impl()
    {
      worker = jthread([this](auto token) { run(token); });
    }

    ~Impl()
    {
      shutdown = true;
      cv.notify_all();
    }

    void run([[maybe_unused]] auto token)
    {
      while (!shutdown) {
        std::unique_lock<std::mutex> lock(mutex);
        if (tasks.empty()) {
          cv.wait(lock, [this] { return !tasks.empty() || shutdown; });
          if (shutdown) break;
        }

        auto now = std::chrono::steady_clock::now();
        if (tasks.top().executeAt <= now) {
          auto task = std::move(const_cast<ScheduledTask &>(tasks.top()));
          tasks.pop();

          auto it = cancelled.find(task.id);
          if (it != cancelled.end() && it->second) {
            cancelled.erase(it);
            continue;
          }

          lock.unlock();
          try {
            task.task();
          } catch (...) {}
          lock.lock();
        } else {
          cv.wait_until(lock, tasks.top().executeAt, [this] { return shutdown.load(); });
        }
      }
    }

    TaskId schedule(Task task, Duration delay)
    {
      std::lock_guard<std::mutex> lock(mutex);
      if (shutdown) return 0;
      TaskId id = nextId++;
      TimePoint executeAt = std::chrono::steady_clock::now() + delay;
      tasks.push({ id, executeAt, std::move(task) });
      cv.notify_one();
      return id;
    }

    void cancel(TaskId id)
    {
      if (id == 0) return;
      std::lock_guard<std::mutex> lock(mutex);
      cancelled[id] = true;
    }
  };

  TaskScheduler();
  ~TaskScheduler() = default;

  [[nodiscard]] TaskId schedule(Task task, Duration delay = {});
  void cancel(TaskId id);
  static TaskScheduler &instance();

private:
  std::unique_ptr<Impl> m_impl;
};

}// namespace CodeWizard::Core
