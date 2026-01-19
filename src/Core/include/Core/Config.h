// include/Core/Config.h
#pragma once

#include "Types.h"
#include <string>
#include <unordered_map>
#include <optional>

namespace CodeWizard::Core {

class Config {
public:
    void setString(std::string_view key, std::string value);
    void setInt(std::string_view key, int value);
    void setBool(std::string_view key, bool value);

    [[nodiscard]] std::optional<std::string> getString(std::string_view key) const;
    [[nodiscard]] std::optional<int> getInt(std::string_view key) const;
    [[nodiscard]] std::optional<bool> getBool(std::string_view key) const;

    static Config& instance();

private:
    std::unordered_map<std::string, std::string> m_stringValues;
    std::unordered_map<std::string, int> m_intValues;
    std::unordered_map<std::string, bool> m_boolValues;
};

} // namespace CodeWizard::Core
