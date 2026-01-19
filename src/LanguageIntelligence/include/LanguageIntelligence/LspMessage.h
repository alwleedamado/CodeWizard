// include/LanguageIntelligence/LspMessage.h
#pragma once

#include <Core/Types.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace CodeWizard::LanguageIntelligence {

struct LspMessage {
    std::string method;
    nlohmann::json params;
    int id = -1; // -1 for notifications
};

class LspMessageParser {
public:
    void appendData(std::string_view data);
    [[nodiscard]] bool hasMessage() const;
    [[nodiscard]] LspMessage takeMessage();

private:
    std::string m_buffer;
};

} // namespace CodeWizard::LanguageIntelligence
