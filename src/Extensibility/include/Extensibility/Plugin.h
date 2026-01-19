// include/Extensibility/Plugin.h
#pragma once

#include <Core/Types.h>
#include <Platform/Path.h>
#include <memory>

namespace CodeWizard::Extensibility {

class Plugin {
public:
    virtual ~Plugin() = default;
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
    [[nodiscard]] virtual const char* name() const = 0;
};

class PluginManager {
public:
    static PluginManager& instance();
    void loadPlugin(const Platform::Path& pluginPath);
    void unloadAll();

private:
    PluginManager() = default;
};

} // namespace CodeWizard::Extensibility
