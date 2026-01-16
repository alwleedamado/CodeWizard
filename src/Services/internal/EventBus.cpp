#include "EventBus.h"
namespace CodeWizard::Services::Internal {
void EventBus::subscribe(const std::string& topic, std::function<void()> callback) {
    m_subscribers[topic].push_back(std::move(callback));
}
void EventBus::publish(const std::string& topic) {
    auto it = m_subscribers.find(topic);
    if (it != m_subscribers.end()) {
        for (auto &cb : it->second) cb();
    }
}
}
