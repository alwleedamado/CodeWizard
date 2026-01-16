#pragma once

#include <memory>
#include "Runtime/IDebugSession.h"
#include "Runtime/IDebugger.h"

namespace CodeWizard::Runtime::Internal {

/**
 * @brief Concrete debug session implementation.
 */
class DebugSession final : public IDebugSession
{
public:
    explicit DebugSession(std::shared_ptr<IDebugger> debugger);

    void start() override;
    void terminate() override;
    bool isRunning() const override;

private:
    std::shared_ptr<IDebugger> m_debugger;
    bool m_running{false};
};

} // namespace CodeWizard::Runtime::Internal

