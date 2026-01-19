// src/Plugin.cpp
#include "Extensibility/Plugin.h"
#include <dlfcn.h>
#include <vector>

namespace CodeWizard::Extensibility {

PluginManager& PluginManager::instance() {
    static PluginManager inst;
    return inst;
}

void PluginManager::loadPlugin(const Platform::Path& pluginPath) {
    // Implementation would use dlopen/dlsym
    // For now, just a placeholder
}

void PluginManager::unloadAll() {
    // Implementation would close all handles
}

} // namespace CodeWizard::Extensibility
