#pragma once

#include <functional>
#include <memory>
#include <chrono>

namespace CodeWizard::Core {

class Timer {
public:
  using Clock = std::chrono::steady_clock;
  using TimePoint = Clock::time_point;
  using Duration = Clock::duration;

  // Create timer with delay
  template<typename Rep, typename Period>
  static std::unique_ptr<Timer> singleShot(
      const std::chrono::duration<Rep, Period>& delay,
      std::function<void()> callback
  );

  // Create repeating timer
  template<typename Rep, typename Period>
  static std::unique_ptr<Timer> repeat(
      const std::chrono::duration<Rep, Period>& interval,
      std::function<void()> callback
  );

  ~Timer();

  // Control methods
  void start();
  void stop();
  [[nodiscard]] bool isActive() const;

  // For repeating timers
  void setRepeatInterval(std::chrono::milliseconds interval);
  [[nodiscard]] std::chrono::milliseconds repeatInterval() const;

  Timer(Duration delay, std::function<void()> callback, bool repeating);
private:

  struct Impl;
  std::unique_ptr<Impl> m_impl;
};

} // namespace CodeWizard::Core