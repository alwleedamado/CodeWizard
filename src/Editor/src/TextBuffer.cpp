// src/TextBuffer.cpp
#include "Editor/TextBuffer.h"
#include <algorithm>
#include <stdexcept>

namespace CodeWizard::Editor {

TextBuffer::TextBuffer() { updateLineEnds(); }

TextBuffer::TextBuffer(std::string_view text) : m_text(text) { updateLineEnds(); }

std::string TextBuffer::line(uint32_t lineIndex) const
{
  if (lineIndex >= m_lineEnds.size()) { throw std::out_of_range("Line index out of range"); }
  size_t start = (lineIndex == 0) ? 0 : m_lineEnds[lineIndex - 1] + 1;
  size_t end = m_lineEnds[lineIndex];
  return m_text.substr(start, end - start);
}

Core::Position TextBuffer::positionFromOffset(size_t offset) const
{
  if (offset > m_text.size()) { throw std::out_of_range("Offset out of range"); }
  auto it = std::ranges::upper_bound(m_lineEnds, offset);
  uint32_t line = static_cast<uint32_t>(it - m_lineEnds.begin());
  size_t lineStart = (line == 0) ? 0 : m_lineEnds[line - 1] + 1;
  uint32_t character = static_cast<uint32_t>(offset - lineStart);
  return { line, character };
}

size_t TextBuffer::offsetFromPosition(Core::Position pos) const
{
  if (pos.line > m_lineEnds.size()) { throw std::out_of_range("Line out of range"); }
  size_t lineStart = (pos.line == 0) ? 0 : m_lineEnds[pos.line - 1] + 1;
  size_t offset = lineStart + pos.character;
  if (offset > m_text.size()) { throw std::out_of_range("Character out of range"); }
  return offset;
}

void TextBuffer::insertText(Core::Position pos, std::string_view text)
{
  size_t offset = offsetFromPosition(pos);
  m_text.insert(offset, text);
  updateLineEnds();
}

void TextBuffer::removeText(Core::TextRange range)
{
  if (!range.valid()) return;
  size_t start = offsetFromPosition(range.start);
  size_t end = offsetFromPosition(range.end);
  if (end > m_text.size()) end = m_text.size();
  m_text.erase(start, end - start);
  updateLineEnds();
}

std::string_view TextBuffer::lineView(uint32_t lineIndex) const
{
  if (lineIndex >= m_lineEnds.size()) {
    static std::string_view empty;
    return empty;
  }
  size_t start = (lineIndex == 0) ? 0 : m_lineEnds[lineIndex - 1] + 1;
  size_t end = m_lineEnds[lineIndex];
  return std::string_view(m_text).substr(start, end - start);
}

void TextBuffer::updateLineEnds()
{
  m_lineEnds.clear();
  if (m_text.empty()) {
    m_lineEnds.push_back(0);// Empty buffer has one empty line
    return;
  }
  size_t pos = 0;
  while ((pos = m_text.find('\n', pos)) != std::string::npos) {
    m_lineEnds.push_back(pos);
    pos++;
  }
  if (m_text.back() != '\n') { m_lineEnds.push_back(m_text.size()); }
}

}// namespace CodeWizard::Editor
