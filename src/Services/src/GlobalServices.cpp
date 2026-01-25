// src/GlobalServices.cpp
#include "Services/GlobalServices.h"
#include <Core/Config.h>
#include <Core/EventBus.h>
#include <Core/Logger.h>
#include <memory>

namespace CodeWizard::Services {

namespace {
  bool g_initialized = false;
}

void initializeDefaultServices()
{
  if (g_initialized) return;

  ServiceRegistry::registerService<LoggerService>(std::make_unique<LoggerService>());
  ServiceRegistry::registerService<ConfigService>(std::make_unique<ConfigService>());

  g_initialized = true;
}

LoggerService *getLogger()
{
  auto *logger = ServiceRegistry::getService<LoggerService>();
  return logger;
}

ConfigService *getConfig() { return ServiceRegistry::getService<ConfigService>(); }

}// namespace CodeWizard::Services
