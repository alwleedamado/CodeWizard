#pragma once
#include "Platform/IThreading.h"
#include <thread>
namespace CodeWizard::Platform::Internal {
class Threading final : public IThreading {
public:
    void runAsync(std::function<void()> task) override;
};
}
