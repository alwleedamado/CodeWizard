#include "Threading.h"
namespace CodeWizard::Platform::Internal {
void Threading::runAsync(std::function<void()> task) {
    std::thread(std::move(task)).detach();
}
}
