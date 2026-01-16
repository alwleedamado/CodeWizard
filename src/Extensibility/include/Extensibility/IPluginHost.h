#pragma once

#include <memory>
#include <vector>

namespace CodeWizard::Extensibility {

class IPlugin;

/**
 * @brief Host environment for all plugins.
 *
 * Plugins use this interface to query services provided by the IDE.
 */
class IPluginHost
{
public:
    virtual ~IPluginHost() = default;

    /**
     * @brief Register a plugin with the host.
     */
    virtual void registerPlugin(std::shared_ptr<IPlugin> plugin) = 0;

    /**
     * @brief Get all registered plugins.
     */
    virtual std::vector<std::shared_ptr<IPlugin>> plugins() const = 0;

    /**
     * @brief Access IDE services (example: event bus, task scheduler)
     * @note Returns null if service is not available
     */
    virtual void* service(const std::string& serviceName) = 0;
};

} // namespace CodeWizard::Extensibility
