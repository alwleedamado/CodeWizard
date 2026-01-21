#include "Runtime/Application.h"
#include <Services/GlobalServices.h>

namespace CodeWizard::Runtime {

int Application::run(int argc, char* argv[]) {
    Services::initializeDefaultServices();
    return m_ui.run(argc, argv);
}

Application& Application::instance() {
    static Application app;
    return app;
}

} // namespace CodeWizard::Runtime
