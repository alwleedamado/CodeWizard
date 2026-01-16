#pragma once

#include <memory>
#include <string>

namespace CodeWizard::Runtime {

class IDebugSession;

/**
 * @brief High-level runtime orchestrator.
 *
 * Bridges BuildRun and Debugger subsystems.
 */
class IRuntimeController
{
public:
    virtual ~IRuntimeController() = default;

    virtual std::shared_ptr<IDebugSession>
    run(const std::string& targetName) = 0;

    virtual std::shared_ptr<IDebugSession>
    debug(const std::string& targetName) = 0;
};

} // namespace CodeWizard::Runtime

