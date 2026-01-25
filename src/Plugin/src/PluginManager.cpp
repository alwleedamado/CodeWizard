#include "Plugin/PluginManager.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

using namespace CodeWizard::Plugin;

PluginManager& PluginManager::instance() {
    static PluginManager manager;
    return manager;
}

PluginManager::PluginManager() {
    // QObject setup
}

void PluginManager::loadBuiltInPlugins() {
    // Register built-in plugins here
    // Example:
    // auto cmakePlugin = std::make_unique<CMakeCppPlugin>();
    // registerPlugin({std::move(cmakePlugin), std::nullopt});
}

void PluginManager::loadPluginsFromDirectory(const QString& directory) {
    QDir dir(directory);
    if (!dir.exists()) return;

#ifdef _WIN32
    QStringList filters = {"*.dll"};
#else
    QStringList filters = {"*.so"};
#endif

    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);

    for (const auto& fileInfo : dir.entryInfoList()) {
        loadPlugin(fileInfo.absoluteFilePath());
    }
}

bool PluginManager::loadPlugin(const QString& pluginPath) {
    try {
        DynamicLibrary library(pluginPath.toStdString());

        // Check API version compatibility
        auto getVersion = library.getSymbol<GetApiVersionFunc>("get_api_version");
        if (!getVersion) {
            qWarning() << "Plugin missing get_api_version symbol:" << pluginPath;
            return false;
        }

        int pluginApiVersion = (*getVersion)();
        if (pluginApiVersion != CODEWIZARD_PLUGIN_API_VERSION) {
            qWarning() << "Plugin API version mismatch:" << pluginPath
                      << "Expected:" << CODEWIZARD_PLUGIN_API_VERSION
                      << "Got:" << pluginApiVersion;
            return false;
        }

        // Get plugin creation functions
        auto createFunc = library.getSymbol<CreatePluginFunc>("create_plugin");
        auto destroyFunc = library.getSymbol<DestroyPluginFunc>("destroy_plugin");

        if (!createFunc || !destroyFunc) {
            qWarning() << "Plugin missing required symbols:" << pluginPath;
            return false;
        }

        // Create plugin instance
        IPlugin* plugin = (*createFunc)();
        if (!plugin) {
            qWarning() << "Plugin creation failed:" << pluginPath;
            return false;
        }

        // Store loaded plugin
        PluginPtr pluginPtr(plugin, [destroyFunc](IPlugin* p) {
            if (p) (*destroyFunc)(p);
        });

        registerPlugin({std::move(pluginPtr), std::move(library)});
        emit pluginLoaded(plugin->name());

        qDebug() << "Loaded plugin:" << plugin->name() << plugin->version();
        return true;

    } catch (const std::exception& e) {
        qWarning() << "Failed to load plugin:" << pluginPath << e.what();
        return false;
    }
}

void PluginManager::registerPlugin(LoadedPlugin&& plugin) {
    plugin.plugin->initialize();
    registerFeatures(plugin.plugin.get());
    m_plugins.push_back(std::move(plugin));
}

void PluginManager::registerFeatures(IPlugin* plugin) {
    auto features = plugin->getFeatures();
    std::lock_guard lock(m_mutex);
    for (auto& feature : features) {
        m_features[feature.type].push_back({plugin, std::move(feature), nullptr});
    }
}

void PluginManager::initializeAllPlugins() {
    for (auto& plugin : m_plugins) {
        plugin.plugin->initialize();
    }
}

void PluginManager::shutdownAllPlugins() {
    for (auto& plugin : m_plugins) {
        plugin.plugin->shutdown();
    }
    m_plugins.clear();
}
std::vector<IPlugin *> PluginManager::getPlugins() const
{
  std::lock_guard lock(m_mutex);
  std::vector<IPlugin*> plugins;
  for (const auto& loaded : m_plugins) {
    plugins.push_back(loaded.plugin.get());
  }
  return plugins;
}

std::vector<IPlugin*> PluginManager::getUiExtensions() {
    std::lock_guard lock(m_mutex);
    std::vector<IPlugin*> results;
    auto it = m_features.find("ui_extension");
    if (it != m_features.end()) {
        for (auto& entry : it->second) {
            results.push_back(entry.plugin);
        }
    }
    return results;
}
