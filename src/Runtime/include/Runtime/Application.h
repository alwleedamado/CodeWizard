#pragma once

#include "UI/UIApplication.h"

namespace CodeWizard::Runtime {

class Application {
public:
    int run(int argc, char* argv[]);
    static Application& instance();

private:
    Application() = default;
    UI::UIApplication m_ui;
};

} // namespace CodeWizard::Runtime
