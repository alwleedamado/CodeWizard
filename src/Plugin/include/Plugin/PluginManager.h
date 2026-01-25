#pragma once

#include "DynamicLibrary.h"
#include "Plugin.h"
#include <QObject>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace CodeWizard::Plugin {

class PluginManager : public QObject
{
  Q_OBJECT

public:
  static PluginManager &instance();

  // Plugin loading
  void loadBuiltInPlugins();
  void loadPluginsFromDirectory(const QString &directory);
  bool loadPlugin(const QString &pluginPath);

  // Feature access
  template<typename T> T *getBestProvider(const QString &featureType);

  template<typename T> std::vector<T *> getAllProviders(const QString &featureType);

  std::vector<IPlugin *> getUiExtensions();

  // Plugin lifecycle
  void initializeAllPlugins();
  void shutdownAllPlugins();

  // Get all loaded plugins
  std::vector<IPlugin *> getPlugins() const;
signals:
  void pluginLoaded(const QString &pluginName);
  void pluginUnloaded(const QString &pluginName);

private:
  PluginManager();

  struct LoadedPlugin
  {
    PluginPtr plugin;
    std::optional<DynamicLibrary> library;// nullopt for built-in plugins
  };

  struct FeatureEntry
  {
    IPlugin *plugin;
    Feature feature;
    mutable QObject *instance = nullptr;
  };

  void registerPlugin(LoadedPlugin &&plugin);
  void registerFeatures(IPlugin *plugin);

  std::vector<LoadedPlugin> m_plugins;
  std::unordered_map<QString, std::vector<FeatureEntry>> m_features;
  mutable std::mutex m_mutex;
};

// Template implementations
template<typename T> T *PluginManager::getBestProvider(const QString &featureType)
{
  std::lock_guard lock(m_mutex);
  auto it = m_features.find(featureType);
  if (it == m_features.end() || it->second.empty()) { return nullptr; }

  auto &entries = it->second;
  // Sort by priority (highest first)
  std::sort(entries.begin(), entries.end(), [](const FeatureEntry &a, const FeatureEntry &b) {
    return a.feature.priority > b.feature.priority;
  });

  return qobject_cast<T *>(entries[0].plugin);
}

template<typename T> std::vector<T *> PluginManager::getAllProviders(const QString &featureType)
{
  std::lock_guard lock(m_mutex);
  std::vector<T *> results;
  auto it = m_features.find(featureType);
  if (it != m_features.end()) {
    for (auto &entry : it->second) {
      if (auto *provider = qobject_cast<T *>(entry.plugin)) { results.push_back(provider); }
    }
  }
  return results;
}

}// namespace CodeWizard::Plugin
