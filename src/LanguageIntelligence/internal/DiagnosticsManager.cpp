#include "DiagnosticsManager.h"

namespace CodeWizard::LanguageIntelligence::Internal {

void DiagnosticsManager::update(std::vector<Diagnostic>&& diagnostics)
{
    m_diagnostics = std::move(diagnostics);
}

const std::vector<Diagnostic>& DiagnosticsManager::current() const
{
    return m_diagnostics;
}

}
