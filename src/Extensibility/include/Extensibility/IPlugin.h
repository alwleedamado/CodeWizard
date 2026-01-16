#pragma once
#include <string>
#include <vector>

namespace CodeWizard::Extensibility {

class IPluginHost; // forward declaration

class IPlugin
{
public:
    virtual ~IPlugin() = default;

    virtual std::string name() const = 0;
    virtual std::string version() const = 0;

    // Dependencies (other plugins this plugin requires)
    virtual std::vector<std::string> dependencies() const = 0;

    // Lifecycle
    virtual bool initialize(IPluginHost* host) = 0;
    virtual void shutdown() = 0;

    // Optional: declare capabilities/features without initializing
    virtual std::vector<std::string> capabilities() const { return {}; }
};

} // namespace CodeWizard::Extensibility
