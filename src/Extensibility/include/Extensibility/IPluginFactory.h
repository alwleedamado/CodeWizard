#pragma once

#include <memory>
#include <string>

namespace CodeWizard::Extensibility {

class IPlugin;
class IPluginHost;

/**
 * @brief Factory interface for creating plugins.
 *
 * Dynamic plugin loaders use this interface to instantiate plugins at runtime.
 */
class IPluginFactory
{
public:
    virtual ~IPluginFactory() = default;

    /**
     * @brief Create a plugin instance.
     * @param host Pointer to plugin host for dependency injection.
     * @return Shared pointer to the plugin instance.
     */
    virtual std::shared_ptr<IPlugin> create(IPluginHost* host) = 0;

    /**
     * @brief Return the unique plugin ID for this factory.
     */
    virtual std::string pluginId() const = 0;
};

} // namespace CodeWizard::Extensibility
