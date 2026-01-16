#pragma once
#include <string>
#include <vector>

namespace CodeWizard::LanguageIntelligence {

struct Diagnostic
{
    std::string message;
    int line;
    int column;
};

class ILanguageIntelligence
{
public:
    virtual ~ILanguageIntelligence() = default;

    virtual void initialize() = 0;
    virtual void shutdown() = 0;

    virtual std::vector<Diagnostic> diagnostics() const = 0;
    virtual void provideCompletions(const std::string& context) = 0;
};

} // namespace CodeWizard::LanguageIntelligence
