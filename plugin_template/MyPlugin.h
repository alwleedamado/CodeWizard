// MyPlugin.h - Plugin implementation
#include <Plugin/Plugin.h>

class MyPlugin : public CodeWizard::Plugin::IPlugin {
public:
    int apiVersion() const override { return CODEWIZARD_PLUGIN_API_VERSION; }
    QString name() const override { return "My Plugin"; }
    QString version() const override { return "1.0.0"; }
    QString description() const override { return "My custom plugin"; }
    std::vector<CodeWizard::Plugin::Feature> getFeatures() override;
    void initialize() override;
    void shutdown() override;
};
