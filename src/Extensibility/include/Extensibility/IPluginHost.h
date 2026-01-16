#pragma once

#include <memory>
#include <vector>
#include <string>

namespace CodeWizard::Extensibility {

class IPlugin;

/**
 * @brief Host environment for all plugins.
 *
 * Plugins use this interface to query services provided by the IDE.
 * Provides safe access to other plugins and IDE services.
 */
class IPluginHost
{
public:
    virtual ~IPluginHost() = default;

    /**
     * @brief Register a plugin with the host.
     * @param plugin The plugin instance to register.
     */
    virtual void registerPlugin(std::shared_ptr<IPlugin> plugin) = 0;

    /**
     * @brief Get all registered plugins.
     * @return Vector of shared_ptr to all plugins.
     */
    virtual std::vector<std::shared_ptr<IPlugin>> plugins() const = 0;

    /**
     * @brief Access IDE services by name.
     * @param serviceName Name of the service (example: "EventBus", "TaskScheduler")
     * @return Pointer to service, nullptr if unavailable
     */
    virtual void* service(const std::string& serviceName) = 0;

    /**
     * @brief Retrieve a plugin by name.
     * @param pluginName Name of the plugin to retrieve.
     * @return Shared pointer to plugin, nullptr if not found
     */
    virtual std::shared_ptr<IPlugin> getPlugin(const std::string& pluginName) const = 0;

    /**
     * @brief Optional: Notify host that plugin is initialized.
     * @param plugin Plugin instance
     */
    virtual void notifyInitialized(std::shared_ptr<IPlugin> plugin) = 0;
};

} // namespace CodeWizard::Extensibility
