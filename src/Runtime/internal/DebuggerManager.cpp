#include "DebuggerManager.h"

namespace CodeWizard::Runtime::Internal {

void DebuggerManager::registerDebugger(std::shared_ptr<IDebugger> debugger)
{
    if (!debugger) return;
    m_debuggers[debugger->name()] = std::move(debugger);
}

std::shared_ptr<IDebugger>
DebuggerManager::debugger(const std::string& name) const
{
    auto it = m_debuggers.find(name);
    return it != m_debuggers.end() ? it->second : nullptr;
}

} // namespace CodeWizard::Runtime::Internal

