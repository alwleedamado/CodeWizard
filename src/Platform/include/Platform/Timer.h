// include/Platform/Timer.h
#pragma once

#include <Core/Types.h>
#include <chrono>
#include <functional>
#include <memory>

namespace CodeWizard::Platform {

class Timer {
public:
    using Callback = std::function<void()>;
    using Duration = std::chrono::milliseconds;

    explicit Timer(Duration interval, Callback callback);
    ~Timer();

    void start();
    void stop();
    [[nodiscard]] bool isActive() const noexcept;

    void setInterval(Duration interval);
    [[nodiscard]] Duration interval() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace CodeWizard::Platform
