#include "LSPClient.h"

namespace CodeWizard::LanguageIntelligence::Internal {

bool LSPClient::startServer(const std::string&)
{
    return true;
}

void LSPClient::stopServer()
{
}

void LSPClient::sendRequest(const std::string&)
{
}

std::string LSPClient::receiveResponse()
{
    return {};
}

}
