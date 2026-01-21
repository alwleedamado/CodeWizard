// include/Core/Logger.h
#pragma once

#include "Types.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string_view>
namespace CodeWizard::Core {
enum class LogLevel : uint8_t { Trace, Debug, Info, Warn, Error, Critical };
}
static std::ostream &operator<<(std::ostream &os, CodeWizard::Core::LogLevel level)
{
  switch (level) {
  case CodeWizard::Core::LogLevel::Trace:
    return os << "Trace";
  case CodeWizard::Core::LogLevel::Debug:
    return os << "Debug";
  case CodeWizard::Core::LogLevel::Info:
    return os << "Info";
  case CodeWizard::Core::LogLevel::Warn:
    return os << "Warn";
  case CodeWizard::Core::LogLevel::Error:
    return os << "Error";
  case CodeWizard::Core::LogLevel::Critical:
    return os << "Critical";
  default:
    return os;
  }
}
namespace CodeWizard::Core {

inline void consoleLogger(LogLevel level, const char *file, int line, const char *function, std::string_view message)
{
  std::cout << "[" << level << "]" << " " << file << ":" << line << function << "()" << " " << message << "\n";
}

class Logger
{
public:
  using LogCallback =
    std::function<void(LogLevel level, const char *file, int line, const char *function, std::string_view message)>;
  explicit Logger(LogCallback logCallback = consoleLogger);

  void setCallback(LogCallback callback);
  void log(LogLevel level, const char *file, int line, const char *function, std::string_view message);

private:
  LogCallback m_callback = nullptr;
  std::mutex m_mutex;
};


// User-friendly macros
#define CW_LOG_TRACE(msg)                                                                   \
  do {                                                                                      \
    ::CodeWizard::Core::Logger logger;                                                      \
    logger.log(::CodeWizard::Core::LogLevel::Trace, __FILE__, __LINE__, __FUNCTION__, msg); \
  } while (false);

#define CW_LOG_DEBUG(msg)                                                                   \
  do {                                                                                      \
    ::CodeWizard::Core::Logger logger;                                                      \
    logger.log(::CodeWizard::Core::LogLevel::Debug, __FILE__, __LINE__, __FUNCTION__, msg); \
  } while (false);

#define CW_LOG_INFO(msg)                                                                   \
  do {                                                                                     \
    ::CodeWizard::Core::Logger logger;                                                     \
    logger.log(::CodeWizard::Core::LogLevel::Info, __FILE__, __LINE__, __FUNCTION__, msg); \
  } while (false);

#define CW_LOG_WARN(msg)                                                                   \
  do {                                                                                     \
    ::CodeWizard::Core::Logger logger;                                                     \
    logger.log(::CodeWizard::Core::LogLevel::Warn, __FILE__, __LINE__, __FUNCTION__, msg); \
  } while (false);

#define CW_LOG_ERROR(msg)                                                                   \
  do {                                                                                      \
    ::CodeWizard::Core::Logger logger;                                                      \
    logger.log(::CodeWizard::Core::LogLevel::Error, __FILE__, __LINE__, __FUNCTION__, msg); \
  } while (false);

}// namespace CodeWizard::Core