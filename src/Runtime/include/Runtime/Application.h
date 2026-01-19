// include/Runtime/Application.h
#pragma once

#include <Core/Types.h>
#include <memory>

namespace CodeWizard::Runtime {

class Application {
public:
    int run(int argc, char* argv[]);
    static Application& instance();

private:
    Application() = default;
};

} // namespace CodeWizard::Runtime
