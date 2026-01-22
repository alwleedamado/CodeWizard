// include/Editor/TextBuffer.h
#pragma once

#include <Core/Types.h>
#include <string>
#include <vector>
#include <string_view>

namespace CodeWizard::Editor {

class TextBuffer {
public:
  TextBuffer() = default;
  explicit TextBuffer(std::string text);

  // Text access
  [[nodiscard]] const std::string& text() const noexcept { return m_text; }
  [[nodiscard]] std::string_view lineView(uint32_t lineIndex) const;
  [[nodiscard]] uint32_t lineCount() const noexcept;

  // Position/offset conversion
  [[nodiscard]] Core::Position positionFromOffset(size_t offset) const;
  [[nodiscard]] size_t offsetFromPosition(Core::Position pos) const;

  // Text modification (incremental)
  void insertAt(size_t offset, std::string_view text);
  void removeRange(size_t startOffset, size_t endOffset);
  void setText(std::string_view text);

  // Line-based access
  [[nodiscard]] std::string_view getLine(uint32_t line) const;
  [[nodiscard]] size_t getLineStartOffset(uint32_t line) const;
  [[nodiscard]] size_t getLineEndOffset(uint32_t line) const;
private:
  void updateLineEnds() const;
  void invalidateLineCache() const;

  std::string m_text;
  mutable std::vector<size_t> m_lineEnds;
  mutable bool m_lineCacheValid = false;
};

} // namespace CodeWizard::Editor