#pragma once
#include <functional>
namespace CodeWizard::Platform {
class IThreading {
public:
    virtual ~IThreading() = default;
    virtual void runAsync(std::function<void()> task) = 0;
};
}
