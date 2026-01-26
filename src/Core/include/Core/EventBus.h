#pragma once

#include <array>
#include <atomic>
#include <functional>
#include <memory>
#include <type_traits>
namespace CodeWizard::Core {
// Event traits - user specializes this
template<typename T> struct EventTraits
{
  static constexpr bool kThreadSafe = false;
};

// Forward declarations
template<typename Ev> class EventBus;

// Global event bus access
template<typename Ev> static EventBus<Ev> &getEventBus()
{
  static EventBus<Ev> bus;
  return bus;
}

// Main EventBus template - one per event type
template<typename Ev> class EventBus
{
public:
  // RAII subscription handle
  class Handle
  {
  public:
    Handle() = default;
    explicit Handle(size_t id) : id_(id) {}
    ~Handle()
    {
      if (id_ != 0) unsubscribe();
    }
    Handle(Handle &&other) noexcept : id_(other.id_) { other.id_ = 0; }
    Handle &operator=(Handle &&other) noexcept
    {
      if (this != &other) {
        if (id_ != 0) unsubscribe();
        id_ = other.id_;
        other.id_ = 0;
      }
      return *this;
    }
    Handle(const Handle &) = delete;
    Handle &operator=(const Handle &) = delete;

  private:
    void unsubscribe() { getEventBus<Ev>().unsubscribe(id_); }
    size_t id_ = 0;
  };

  // Post event - lock-free if thread-safe
  static void post(Ev &&ev)
  {
    if constexpr (EventTraits<Ev>::kThreadSafe) {
      instance().template push<true>(std::forward<Ev>(ev));
    } else {
      instance().template push<false>(std::forward<Ev>(ev));
    }
  }

  // Subscribe - returns RAII handle
  template<typename F> static Handle subscribe(F &&f) { return instance().template doSubscribe<F>(std::forward<F>(f)); }

  // Poll - call from main thread
  static void poll() { instance().drain(); }

private:
  EventBus() = default;
  static EventBus &instance()
  {
    static EventBus bus;
    return bus;
  }

  // Fixed-size ring buffer (power of 2)
  static constexpr size_t kBufferSize = 1024;
  alignas(64) std::array<std::aligned_storage_t<sizeof(Ev), alignof(Ev)>, kBufferSize> buffer_;
  std::atomic<size_t> writeIndex_{ 0 };
  std::atomic<size_t> readIndex_{ 0 };

  // Callbacks
  static constexpr size_t kMaxListeners = 64;
  std::array<std::function<void(const Ev &)>, kMaxListeners> listeners_;
  std::atomic<size_t> listenerCount_{ 0 };
  std::mutex listenerMutex_;// Only used during subscribe/unsubscribe

  // Thread-safe push
  template<bool ThreadSafe> void push(Ev &&ev)
  {
    if constexpr (ThreadSafe) {
      // Lock-free SPSC queue
      size_t writeIdx = writeIndex_.load(std::memory_order_relaxed);
      size_t nextWrite = (writeIdx + 1) & (kBufferSize - 1);

      // Check for overflow (drop oldest)
      if (nextWrite == readIndex_.load(std::memory_order_acquire)) {
        readIndex_.fetch_add(1, std::memory_order_relaxed);// Drop oldest
      }

      new (&buffer_[writeIdx]) Ev(std::forward<Ev>(ev));
      writeIndex_.store(nextWrite, std::memory_order_release);
    } else {
      // Single-threaded - just store directly
      size_t writeIdx = writeIndex_.load(std::memory_order_relaxed);
      size_t nextWrite = (writeIdx + 1) & (kBufferSize - 1);

      if (nextWrite == readIndex_.load(std::memory_order_relaxed)) {
        // Drain immediately for non-thread-safe events
        drain();
        nextWrite = (writeIndex_.load(std::memory_order_relaxed) + 1) & (kBufferSize - 1);
      }

      new (&buffer_[writeIdx]) Ev(std::forward<Ev>(ev));
      writeIndex_.store(nextWrite, std::memory_order_relaxed);
    }
  }

  template<typename F> Handle doSubscribe(F &&f)
  {
    std::lock_guard<std::mutex> lock(listenerMutex_);
    size_t count = listenerCount_.load(std::memory_order_relaxed);
    if (count >= kMaxListeners) return Handle{};

    listeners_[count] = std::forward<F>(f);
    listenerCount_.store(count + 1, std::memory_order_release);
    return Handle{ count + 1 };
  }

  void unsubscribe(size_t id)
  {
    std::lock_guard<std::mutex> lock(listenerMutex_);
    size_t index = id - 1;
    if (index < listenerCount_.load(std::memory_order_relaxed)) { listeners_[index] = nullptr; }
  }

  void drain()
  {
    size_t readIdx = readIndex_.load(std::memory_order_relaxed);
    size_t writeIdx = writeIndex_.load(std::memory_order_acquire);

    while (readIdx != writeIdx) {
      Ev &event = *reinterpret_cast<Ev *>(&buffer_[readIdx]);

      // Notify all listeners
      size_t count = listenerCount_.load(std::memory_order_acquire);
      for (size_t i = 0; i < count; ++i) {
        if (listeners_[i]) { listeners_[i](event); }
      }

      event.~Ev();
      readIdx = (readIdx + 1) & (kBufferSize - 1);
    }

    readIndex_.store(readIdx, std::memory_order_release);
  }
};

template<typename Ev>
void postEvent(Ev&& ev)
{
  EventBus<std::decay_t<Ev>>::post(std::forward<Ev>(ev));
}
}// namespace CodeWizard::Core