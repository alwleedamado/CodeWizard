#pragma once

#include <string>
#include "LanguageIntelligence/ILanguageServer.h"

namespace CodeWizard::LanguageIntelligence::Internal {

/**
 * @brief Generic Language Server Protocol client.
 *
 * Handles JSON-RPC transport and process management.
 */
class LSPClient : public ILanguageServer
{
public:
    bool startServer(const std::string& executablePath) override;
    void stopServer() override;

    void sendRequest(const std::string& jsonRequest) override;
    std::string receiveResponse() override;
};

}
