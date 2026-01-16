#pragma once

#include <memory>
#include <string>
#include <vector>

namespace CodeWizard::Extensibility {

class IPlugin;
class IPluginHost;

/**
 * @brief Factory interface for creating plugins dynamically.
 *
 * Used by PluginLoader to instantiate plugins at runtime.
 * Separates the act of loading a plugin library from creating the plugin instance.
 */
class IPluginFactory
{
public:
    virtual ~IPluginFactory() = default;

    /**
     * @brief Create a plugin instance.
     * @param host Pointer to the plugin host for dependency injection and service access.
     * @return Shared pointer to the plugin instance.
     */
    virtual std::shared_ptr<IPlugin> create(IPluginHost* host) = 0;

    /**
     * @brief Return the unique plugin ID for this factory.
     * Used to match dependencies and identify plugins.
     */
    virtual std::string pluginId() const = 0;

    /**
     * @brief Return the version of the plugin this factory produces.
     */
    virtual std::string version() const { return "0.1"; }

    /**
     * @brief Return the list of dependencies (plugin IDs) required by this plugin.
     */
    virtual std::vector<std::string> dependencies() const { return {}; }

    /**
     * @brief Return optional capabilities / features of this plugin without instantiation.
     */
    virtual std::vector<std::string> capabilities() const { return {}; }
};

} // namespace CodeWizard::Extensibility
