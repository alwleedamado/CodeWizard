#pragma once
#include <functional>
#include <string>
namespace CodeWizard::Services {
class IEventBus {
public:
    virtual ~IEventBus() = default;
    virtual void subscribe(const std::string& topic, std::function<void()> callback) = 0;
    virtual void publish(const std::string& topic) = 0;
};
}
