// src/Application.cpp
#include "Runtime/Application.h"
#include <Services/GlobalServices.h>

namespace CodeWizard::Runtime {

int Application::run(int argc, char* argv[]) {
    Services::initializeDefaultServices();
    // Actual implementation would start the UI event loop
    return 0;
}

Application& Application::instance() {
    static Application app;
    return app;
}

} // namespace CodeWizard::Runtime
