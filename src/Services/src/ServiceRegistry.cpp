// src/ServiceRegistry.cpp
#include "Services/ServiceRegistry.h"
#include "Services/GlobalServices.h"
#include <typeinfo>

namespace CodeWizard::Services {

// Explicit instantiation to avoid linker issues
template<>
void ServiceRegistry::registerService<LoggerService>(std::unique_ptr<LoggerService> service);

template<>
void ServiceRegistry::registerService<ConfigService>(std::unique_ptr<ConfigService> service);

template<>
void ServiceRegistry::registerService<EventBusService>(std::unique_ptr<EventBusService> service);

template<>
void ServiceRegistry::registerService<TaskSchedulerService>(std::unique_ptr<TaskSchedulerService> service);

} // namespace CodeWizard::Services
