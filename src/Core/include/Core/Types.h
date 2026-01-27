// include/Core/Types.h
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace CodeWizard::Core {

// Use std::string — but alias for future flexibility
using String     = std::string;
using StringView = std::string_view;

// Position in text buffer (0-based)
struct Position
{
  uint32_t line      = 0;// 0-based
  uint32_t character = 0;// UTF-8 byte offset


  constexpr bool operator==(const Position &other) const = default;
  constexpr bool operator!=(const Position &other) const = default;
  constexpr bool operator<(const Position &other) const
  {
    return line < other.line || (line == other.line && character < other.character);
  }
  constexpr bool operator<=(const Position &other) const { return *this < other || *this == other; }
  constexpr bool operator>(const Position &other) const { return !(*this <= other); }
  constexpr bool operator>=(const Position &other) const { return !(*this < other); }
};

// Range in text buffer
struct TextRange
{
  Position start;
  Position end;

  [[nodiscard]] constexpr bool empty() const { return start == end; }

  [[nodiscard]] constexpr bool valid() const { return start <= end; }
};

// Unique ID for documents, tasks, etc.
using Id = uint64_t;

}// namespace CodeWizard::Core
