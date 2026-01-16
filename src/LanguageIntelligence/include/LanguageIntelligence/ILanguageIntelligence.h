#pragma once

namespace CodeWizard::LanguageIntelligence {

/**
 * @brief Public interface for the LanguageIntelligence subsystem.
 *
 * Architectural constraints:
 * - No UI dependencies unless this is the UI module
 * - No concrete implementations exposed
 * - No cross-layer shortcuts
 */
class ILanguageIntelligence
{
public:
    virtual ~ILanguageIntelligence() = default;
};

} // namespace CodeWizard::LanguageIntelligence
