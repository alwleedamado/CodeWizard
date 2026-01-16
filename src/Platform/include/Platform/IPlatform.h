#pragma once

#include <string>

namespace CodeWizard::Platform {

/**
 * @brief Cross-platform OS services
 */
class IPlatform
{
public:
    virtual ~IPlatform() = default;

    /**
     * @brief Check if a file exists.
     */
    virtual bool fileExists(const std::string& path) const = 0;

    /**
     * @brief Get the path to the user home directory.
     */
    virtual std::string userHome() const = 0;
};

} // namespace CodeWizard::Platform
