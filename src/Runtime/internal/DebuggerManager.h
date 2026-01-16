#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include "Runtime/IDebugger.h"

namespace CodeWizard::Runtime::Internal {

/**
 * @brief Manages available debugger implementations.
 */
class DebuggerManager
{
public:
    void registerDebugger(std::shared_ptr<IDebugger> debugger);
    std::shared_ptr<IDebugger> debugger(const std::string& name) const;

private:
    std::unordered_map<std::string, std::shared_ptr<IDebugger>> m_debuggers;
};

} // namespace CodeWizard::Runtime::Internal

