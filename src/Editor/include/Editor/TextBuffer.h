// include/Editor/TextBuffer.h
#pragma once

#include "Core/Undo.h"


#include <Core/Result.h>
#include <Core/Types.h>
#include <string>
#include <vector>

namespace CodeWizard::Editor {

class TextBuffer
{
public:
  TextBuffer();
  explicit TextBuffer(std::string_view text);

  // Text access
  [[nodiscard]] const std::string &text() const noexcept { return m_text; }
  [[nodiscard]] std::string line(uint32_t lineIndex) const;
  [[nodiscard]] uint32_t lineCount() const noexcept { return static_cast<uint32_t>(m_lineEnds.size()); }

  // Position utilities
  [[nodiscard]] Core::Position positionFromOffset(size_t offset) const;
  [[nodiscard]] size_t offsetFromPosition(Core::Position pos) const;
  [[nodiscard]] static Core::Position computeEndPosition(Core::Position start, std::string_view text);
  // Mutations
  void insertText(Core::Position pos, std::string_view text);
  void removeText(Core::TextRange range);

  // Line-based access
  [[nodiscard]] std::string_view lineView(uint32_t lineIndex) const;

private:
  void updateLineEnds();
  std::string m_text;
  std::vector<size_t> m_lineEnds;// offsets of '\n' characters
  Core::UndoRedoStack m_undoRedoStack;
};

}// namespace CodeWizard::Editor
