// include/Core/Logger.h
#pragma once

#include "Types.h"
#include <string_view>
#include <functional>
#include <memory>

namespace CodeWizard::Core {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical
};

class Logger {
public:
    using LogCallback = std::function<void(LogLevel level, const char* file, int line, const char* function, std::string_view message)>;

    static void setGlobalCallback(LogCallback callback);
    static void log(LogLevel level, const char* file, int line, const char* function, std::string_view message);

    // Convenience macros will be defined below
};

// User-friendly macros
#define CW_LOG_TRACE(msg) ::CodeWizard::Core::Logger::log(::CodeWizard::Core::LogLevel::Trace, __FILE__, __LINE__, __FUNCTION__, msg)
#define CW_LOG_DEBUG(msg) ::CodeWizard::Core::Logger::log(::CodeWizard::Core::LogLevel::Debug, __FILE__, __LINE__, __FUNCTION__, msg)
#define CW_LOG_INFO(msg)  ::CodeWizard::Core::Logger::log(::CodeWizard::Core::LogLevel::Info,  __FILE__, __LINE__, __FUNCTION__, msg)
#define CW_LOG_WARN(msg)  ::CodeWizard::Core::Logger::log(::CodeWizard::Core::LogLevel::Warn,  __FILE__, __LINE__, __FUNCTION__, msg)
#define CW_LOG_ERROR(msg) ::CodeWizard::Core::Logger::log(::CodeWizard::Core::LogLevel::Error, __FILE__, __LINE__, __FUNCTION__, msg)

} // namespace CodeWizard::Core
