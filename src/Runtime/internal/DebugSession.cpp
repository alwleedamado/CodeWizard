#include "DebugSession.h"

namespace CodeWizard::Runtime::Internal {

DebugSession::DebugSession(std::shared_ptr<IDebugger> debugger)
    : m_debugger(std::move(debugger))
{
}

void DebugSession::start()
{
    if (m_debugger)
        m_running = true;
}

void DebugSession::terminate()
{
    if (m_debugger)
        m_debugger->stop();

    m_running = false;
}

bool DebugSession::isRunning() const
{
    return m_running;
}

} // namespace CodeWizard::Runtime::Internal

