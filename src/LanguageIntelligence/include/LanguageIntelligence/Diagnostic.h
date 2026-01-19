// include/LanguageIntelligence/Diagnostic.h
#pragma once

#include <Core/Types.h>
#include <string>

namespace CodeWizard::LanguageIntelligence {

enum class DiagnosticSeverity {
    Error = 1,
    Warning = 2,
    Information = 3,
    Hint = 4
};

struct Diagnostic {
    Core::TextRange range;
    DiagnosticSeverity severity;
    std::string message;
    std::string source; // e.g., "clangd"
};

} // namespace CodeWizard::LanguageIntelligence
