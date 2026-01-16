#pragma once

#include <vector>
#include "LanguageIntelligence/ILanguageIntelligence.h"

namespace CodeWizard::LanguageIntelligence::Internal {

/**
 * @brief Stores and manages diagnostics.
 *
 * Designed to avoid unnecessary allocations.
 */
class DiagnosticsManager
{
public:
    void update(std::vector<Diagnostic>&& diagnostics);
    const std::vector<Diagnostic>& current() const;

private:
    std::vector<Diagnostic> m_diagnostics;
};

}
