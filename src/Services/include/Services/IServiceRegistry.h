#pragma once
#include <string>
#include <memory>
namespace CodeWizard::Services {
class IServiceRegistry {
public:
    virtual ~IServiceRegistry() = default;
    virtual void registerService(const std::string& name, std::shared_ptr<void> service) = 0;
    virtual std::shared_ptr<void> getService(const std::string& name) = 0;
};
}
