#pragma once

#include <string>

namespace CodeWizard::Extensibility {

/**
 * @brief Represents a generic plugin for CodeWizard.
 *
 * All plugins must implement this interface.
 * Plugins are loaded dynamically at runtime and must not depend
 * on internal module implementations directly.
 */
class IPlugin
{
public:
    virtual ~IPlugin() = default;

    /**
     * @brief Get the unique name of the plugin.
     */
    virtual std::string name() const = 0;

    /**
     * @brief Initialize the plugin.
     *
     * Called once after the plugin is loaded.
     */
    virtual void initialize() = 0;

    /**
     * @brief Shutdown the plugin.
     *
     * Called before the plugin is unloaded.
     */
    virtual void shutdown() = 0;
};

} // namespace CodeWizard::Extensibility
