#pragma once
#include <string>

namespace CodeWizard::LanguageIntelligence {

class ILanguageServer
{
public:
    virtual ~ILanguageServer() = default;

    virtual bool startServer(const std::string& executablePath) = 0;
    virtual void stopServer() = 0;

    virtual void sendRequest(const std::string& jsonRequest) = 0;
    virtual std::string receiveResponse() = 0;
};

} // namespace CodeWizard::LanguageIntelligence
