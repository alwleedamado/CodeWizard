#pragma once
#include "Services/IEventBus.h"
#include <unordered_map>
#include <vector>
namespace CodeWizard::Services::Internal {
class EventBus final : public IEventBus {
public:
    void subscribe(const std::string& topic, std::function<void()> callback) override;
    void publish(const std::string& topic) override;
private:
    std::unordered_map<std::string, std::vector<std::function<void()>>> m_subscribers;
};
}
