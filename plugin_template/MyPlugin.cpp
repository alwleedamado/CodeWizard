// MyPlugin.cpp
#include "MyPlugin.h"

std::vector<CodeWizard::Plugin::Feature> MyPlugin::getFeatures() {
    return {
        {"my-feature", "ui_extension", 50, {}}
    };
}

void MyPlugin::initialize() {
    // Plugin initialization
}

void MyPlugin::shutdown() {
    // Plugin cleanup
}
