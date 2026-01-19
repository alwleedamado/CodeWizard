// src/Config.cpp
#include "Core/Config.h"

namespace CodeWizard::Core {

void Config::setString(std::string_view key, std::string value) {
    m_stringValues[std::string(key)] = std::move(value);
}

void Config::setInt(std::string_view key, int value) {
    m_intValues[std::string(key)] = value;
}

void Config::setBool(std::string_view key, bool value) {
    m_boolValues[std::string(key)] = value;
}

std::optional<std::string> Config::getString(std::string_view key) const {
    auto it = m_stringValues.find(std::string(key));
    return (it != m_stringValues.end()) ? std::make_optional(it->second) : std::nullopt;
}

std::optional<int> Config::getInt(std::string_view key) const {
    auto it = m_intValues.find(std::string(key));
    return (it != m_intValues.end()) ? std::make_optional(it->second) : std::nullopt;
}

std::optional<bool> Config::getBool(std::string_view key) const {
    auto it = m_boolValues.find(std::string(key));
    return (it != m_boolValues.end()) ? std::make_optional(it->second) : std::nullopt;
}

Config& Config::instance() {
    static Config inst;
    return inst;
}

} // namespace CodeWizard::Core
