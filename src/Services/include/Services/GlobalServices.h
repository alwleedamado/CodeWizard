// include/Services/GlobalServices.h
#pragma once

#include "ServiceRegistry.h"
#include <Core/Logger.h>
#include <Core/Config.h>
#include <Core/EventBus.h>
#include <memory>

namespace CodeWizard::Services {

// Predefined service types
using LoggerService = Core::Logger;
using ConfigService = Core::Config;

// Initialize default global services
void initializeDefaultServices();

// Accessors
[[nodiscard]] LoggerService* getLogger();
[[nodiscard]] ConfigService* getConfig();

} // namespace CodeWizard::Services
