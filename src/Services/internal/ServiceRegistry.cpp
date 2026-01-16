#include "ServiceRegistry.h"
namespace CodeWizard::Services::Internal {
void ServiceRegistry::registerService(const std::string& name, std::shared_ptr<void> service) {
    m_services[name] = std::move(service);
}
std::shared_ptr<void> ServiceRegistry::getService(const std::string& name) {
    auto it = m_services.find(name);
    return it != m_services.end() ? it->second : nullptr;
}
}
