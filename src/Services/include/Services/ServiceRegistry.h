// include/Services/ServiceRegistry.h
#pragma once

#include <Core/Types.h>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <functional>

namespace CodeWizard::Services {

class ServiceRegistry {
public:
  template<typename T>
  static void registerService(std::unique_ptr<T> service) {
    auto& instance = getInstance();
    // Store raw pointer + deleter
    instance.m_services[typeid(T)] = ServiceEntry{
      .ptr = service.release(),
      .deleter = [](void* p) { delete static_cast<T*>(p); }
    };
  }

  template<typename T>
  static T* getService() {
    auto& instance = getInstance();
    auto it = instance.m_services.find(typeid(T));
    return (it != instance.m_services.end()) ? static_cast<T*>(it->second.ptr) : nullptr;
  }

  template<typename T>
  static bool hasService() {
    auto& instance = getInstance();
    return instance.m_services.count(typeid(T)) > 0;
  }

private:
  struct ServiceEntry {
    void* ptr = nullptr;
    std::function<void(void*)> deleter;
  };

  ServiceRegistry() = default;
  ~ServiceRegistry() {
    for (auto& [_, entry] : m_services) {
      if (entry.ptr) {
        entry.deleter(entry.ptr);
      }
    }
  }

  static ServiceRegistry& getInstance() {
    static ServiceRegistry inst;
    return inst;
  }

  std::unordered_map<std::type_index, ServiceEntry> m_services;
};

} // namespace CodeWizard::Services