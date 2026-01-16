#pragma once
#include "Services/IServiceRegistry.h"
#include <unordered_map>
#include <string>
namespace CodeWizard::Services::Internal {
class ServiceRegistry final : public IServiceRegistry {
public:
    void registerService(const std::string& name, std::shared_ptr<void> service) override;
    std::shared_ptr<void> getService(const std::string& name) override;
private:
    std::unordered_map<std::string, std::shared_ptr<void>> m_services;
};
}
