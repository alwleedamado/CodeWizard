// plugin_exports.cpp - Must be compiled into the .so/.dll
#include "MyPlugin.h"
#include <memory>

extern "C" {
    CodeWizard::Plugin::IPlugin* create_plugin() {
        return new MyPlugin();
    }

    void destroy_plugin(CodeWizard::Plugin::IPlugin* plugin) {
        delete plugin;
    }

    int get_api_version() {
        return CODEWIZARD_PLUGIN_API_VERSION;
    }
}
