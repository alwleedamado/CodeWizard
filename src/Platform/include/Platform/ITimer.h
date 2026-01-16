#pragma once
#include <functional>
namespace CodeWizard::Platform {
class ITimer {
public:
    virtual ~ITimer() = default;
    virtual void start(int intervalMs, std::function<void()> callback) = 0;
    virtual void stop() = 0;
};
}
