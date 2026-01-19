// include/Core/Result.h
#pragma once

#include <system_error>
#include <variant>

namespace CodeWizard::Core {

enum class ErrorCode {
  Success = 0,
  FileNotFound,
  PermissionDenied,
  InvalidArgument,
  IoError,
  ParseError,
  NotImplemented,
  Unknown
};

class Error
{
public:
  explicit Error() = default;
  explicit Error(ErrorCode code, std::string message = {}) : m_code(code), m_message(std::move(message)) {}

  [[nodiscard]] ErrorCode code() const noexcept { return m_code; }
  [[nodiscard]] const std::string &message() const noexcept { return m_message; }

private:
  ErrorCode m_code;
  std::string m_message;
};

template<typename T> class Result
{
public:
  template<typename U = T,
    std::enable_if_t<!std::is_same_v<std::decay_t<U>, Result<T>> && std::is_constructible_v<T, U>, int> = 0>
  constexpr explicit Result(U &&value) : m_data(std::forward<U>(value))
  {}


  // Construct error
  explicit Result(Error error) : m_data(std::move(error)) {}

  // Observers
  [[nodiscard]] bool hasValue() const noexcept { return std::holds_alternative<T>(m_data); }

  [[nodiscard]] bool hasError() const noexcept { return std::holds_alternative<Error>(m_data); }

  // Accessors (unchecked)
  T &value() & { return std::get<T>(m_data); }
  const T &value() const & { return std::get<T>(m_data); }
  T &&value() && { return std::get<T>(std::move(m_data)); }

  Error &error() & { return std::get<Error>(m_data); }
  const Error &error() const & { return std::get<Error>(m_data); }

  // Safe access
  template<typename U> T valueOr(U &&defaultValue) const &
  {
    return hasValue() ? value() : static_cast<T>(std::forward<U>(defaultValue));
  }

private:
  std::variant<T, Error> m_data;
};

// Specialization for void
template<> class Result<void>
{
public:
  using ValueType = void;
  using ErrorType = Error;

  Result() : m_hasValue(true), m_error() {}
  Result(Error error) : m_hasValue(false), m_error(std::move(error)) {}

  [[nodiscard]] bool hasValue() const noexcept { return m_hasValue; }
  [[nodiscard]] bool hasError() const noexcept { return !m_hasValue; }

  const Error &error() const { return m_error; }

private:
  bool m_hasValue;
  Error m_error;
};


template<typename T> Result<T> success(T value) { return Result<T>(std::move(value)); }

template<typename T> Result<T> failure(ErrorCode code, std::string message = {})
{
  return Result<T>(Error{ code, std::move(message) });
}
// Helper constructors
inline Result<void> success() { return Result<void>{}; }
inline Result<void> failure(ErrorCode code, std::string message = {})
{
  return Result<void>(Error{ code, std::move(message) });
}

}// namespace CodeWizard::Core
