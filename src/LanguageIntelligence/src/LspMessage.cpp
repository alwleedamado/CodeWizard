// src/LspMessage.cpp
#include "LanguageIntelligence/LspMessage.h"
#include <sstream>
#include <stdexcept>

namespace CodeWizard::LanguageIntelligence {

void LspMessageParser::appendData(std::string_view data) {
    m_buffer.append(data);
}

bool LspMessageParser::hasMessage() const {
    if (m_buffer.size() < 4) return false;
    size_t headerEnd = m_buffer.find("\r\n\r\n");
    return headerEnd != std::string::npos;
}

LspMessage LspMessageParser::takeMessage() {
    if (!hasMessage()) {
        throw std::runtime_error("No complete message available");
    }

    size_t headerEnd = m_buffer.find("\r\n\r\n");
    std::string header = m_buffer.substr(0, headerEnd);
    size_t contentLength = 0;

    std::istringstream headerStream(header);
    std::string line;
    while (std::getline(headerStream, line)) {
        if (line.rfind("Content-Length: ", 0) == 0) {
            contentLength = std::stoul(line.substr(16));
            break;
        }
    }

    size_t jsonStart = headerEnd + 4;
    if (m_buffer.size() < jsonStart + contentLength) {
        throw std::runtime_error("Incomplete message body");
    }

    std::string jsonStr = m_buffer.substr(jsonStart, contentLength);
    m_buffer.erase(0, jsonStart + contentLength);

    nlohmann::json json = nlohmann::json::parse(jsonStr);
    LspMessage msg;
    msg.method = json.value("method", "");
    msg.params = json.value("params", nlohmann::json{});
    msg.id = json.contains("id") ? json["id"].get<int>() : -1;

    return msg;
}

} // namespace CodeWizard::LanguageIntelligence
