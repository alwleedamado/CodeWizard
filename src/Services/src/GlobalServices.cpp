// src/GlobalServices.cpp
#include "Services/GlobalServices.h"
#include <Core/Logger.h>
#include <Core/Config.h>
#include <Core/EventBus.h>
#include <Core/Task.h>
#include <memory>

namespace CodeWizard::Services {

namespace {
bool g_initialized = false;
}

void initializeDefaultServices() {
    if (g_initialized) return;

    ServiceRegistry::registerService<LoggerService>(std::make_unique<LoggerService>());
    ServiceRegistry::registerService<ConfigService>(std::make_unique<ConfigService>());
    ServiceRegistry::registerService<EventBusService>(std::make_unique<EventBusService>());
    ServiceRegistry::registerService<TaskSchedulerService>(std::make_unique<TaskSchedulerService>());

    g_initialized = true;
}

LoggerService* getLogger() {
    return ServiceRegistry::getService<LoggerService>();
}

ConfigService* getConfig() {
    return ServiceRegistry::getService<ConfigService>();
}

EventBusService* getEventBus() {
    return ServiceRegistry::getService<EventBusService>();
}

TaskSchedulerService* getTaskScheduler() {
    return ServiceRegistry::getService<TaskSchedulerService>();
}

} // namespace CodeWizard::Services
