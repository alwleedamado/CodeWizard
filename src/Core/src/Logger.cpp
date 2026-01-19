// src/Logger.cpp
#include "Core/Logger.h"
#include <mutex>

namespace CodeWizard::Core {

namespace {
    Logger::LogCallback g_callback = nullptr;
    std::mutex g_callbackMutex;
}

void Logger::setGlobalCallback(LogCallback callback) {
    std::lock_guard lock(g_callbackMutex);
    g_callback = std::move(callback);
}

void Logger::log(LogLevel level, const char* file, int line, const char* function, std::string_view message) {
    std::lock_guard lock(g_callbackMutex);
    if (g_callback) {
        g_callback(level, file, line, function, message);
    }
}

} // namespace CodeWizard::Core
