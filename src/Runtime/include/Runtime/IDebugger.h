#pragma once

#include <string>

namespace CodeWizard::Runtime {

/**
 * @brief Abstract debugger interface.
 *
 * Implemented by debugger plugins (GDB, LLDB, etc.).
 */
class IDebugger
{
public:
    virtual ~IDebugger() = default;

    virtual std::string name() const = 0;

    virtual bool start(const std::string& executablePath) = 0;
    virtual void stop() = 0;

    virtual void setBreakpoint(const std::string& file, int line) = 0;
    virtual void removeBreakpoint(const std::string& file, int line) = 0;

    virtual void continueExecution() = 0;
    virtual void stepOver() = 0;
    virtual void stepInto() = 0;
};

} // namespace CodeWizard::Runtime

