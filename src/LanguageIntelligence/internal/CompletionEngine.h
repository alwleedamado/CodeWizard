#pragma once

#include <string>

namespace CodeWizard::LanguageIntelligence::Internal {

/**
 * @brief Completion provider.
 *
 * Abstracts completion logic from transport (LSP).
 */
class CompletionEngine
{
public:
    void requestCompletions(const std::string& context);
};

}
