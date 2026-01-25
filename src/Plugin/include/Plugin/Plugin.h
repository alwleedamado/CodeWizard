#pragma once

#include <QVariant>
#include <memory>
#include <vector>

// Versioning for ABI compatibility
#define CODEWIZARD_PLUGIN_API_VERSION 1

namespace CodeWizard::Plugin {

// Feature types as strings (more flexible than enums)
struct Feature {
    QString id;
    QString type;        // "project_provider", "language_server", "ui_extension", etc.
    int priority = 0;    // Higher = preferred
    QVariant metadata;   // Plugin-specific data

    bool operator<(const Feature& other) const {
        return priority > other.priority; // For sorting (highest first)
    }
};

class IPlugin {
public:
    virtual ~IPlugin() = default;

    // Required methods
    virtual int apiVersion() const = 0;
    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual QString description() const = 0;
    virtual std::vector<Feature> getFeatures() = 0;
    virtual void initialize() = 0;
    virtual void shutdown() = 0;
};

// C-compatible function signatures for dynamic loading
extern "C" {
    typedef IPlugin* (*CreatePluginFunc)();
    typedef void (*DestroyPluginFunc)(IPlugin* plugin);
    typedef int (*GetApiVersionFunc)();
}

// Smart pointer for plugins
using PluginDeleter = std::function<void(IPlugin*)>;
using PluginPtr = std::unique_ptr<IPlugin, PluginDeleter>;

} // namespace CodeWizard::Plugin
