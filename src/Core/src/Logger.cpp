#include "Core/Logger.h"
#include <mutex>
#include <utility>

namespace CodeWizard::Core {

Logger::Logger(LogCallback logCallback) : m_callback(std::move(logCallback)) {}

void Logger::setCallback(LogCallback callback)
{
  std::lock_guard const lock(m_mutex);
  m_callback = std::move(callback);
}

void Logger::log(LogLevel level, const char *file, int line, const char *function, std::string_view message)
{
  std::lock_guard const lock(m_mutex);
  if (m_callback) { m_callback(level, file, line, function, message); }
}
void Logger::error(const char *str, const std::string &message)
{
  std::lock_guard const lock(m_mutex);
  if (m_callback) { m_callback(LogLevel::Error, "",0, "", message); }
}
}// namespace CodeWizard::Core
