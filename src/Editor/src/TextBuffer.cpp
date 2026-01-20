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
  if (offset > m_text.size()) {
    throw std::out_of_range("Offset out of range");
  }

  // Handle empty buffer
  if (m_text.empty()) {
    return {0, 0};
  }

  // Find the line that contains this offset
  auto it = std::lower_bound(m_lineEnds.begin(), m_lineEnds.end(), offset);

  uint32_t line;
  size_t lineStart;

  if (it == m_lineEnds.end()) {
    // Offset is beyond all newlines → last line
    line = static_cast<uint32_t>(m_lineEnds.size());
    lineStart = (line == 0) ? 0 : m_lineEnds[line - 1] + 1;
  } else if (*it == offset) {
    // Offset is exactly at a newline → belongs to current line
    line = static_cast<uint32_t>(it - m_lineEnds.begin());
    lineStart = (line == 0) ? 0 : m_lineEnds[line - 1] + 1;
  } else {
    // Offset is before a newline → previous line
    line = static_cast<uint32_t>(it - m_lineEnds.begin());
    if (line == 0) {
      lineStart = 0;
    } else {
      lineStart = m_lineEnds[line - 1] + 1;
    }
  }

  uint32_t character = static_cast<uint32_t>(offset - lineStart);
  return {line, character};
}

size_t TextBuffer::offsetFromPosition(Core::Position pos) const
{
  if (pos.line > m_lineEnds.size()) {
    throw std::out_of_range("Line out of range");
  }

  size_t lineStart;
  if (pos.line == 0) {
    lineStart = 0;
  } else {
    // For lines beyond the last newline, use end of buffer
    if (pos.line - 1 >= m_lineEnds.size()) {
      lineStart = m_text.size();
    } else {
      lineStart = m_lineEnds[pos.line - 1] + 1;
    }
  }

  size_t offset = lineStart + pos.character;

  // Clamp to buffer size (for positions beyond actual text)
  if (offset > m_text.size()) {
    offset = m_text.size();
  }

  return offset;
}
Core::Position TextBuffer::computeEndPosition(Core::Position start, std::string_view text)
{
  uint32_t line = start.line;
  uint32_t character = start.character;

  for (const char c : text) {
    if (c == '\n') {
      line++;
      character = 0;
    } else {
      character++;
    }
  }
  return {line, character};
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
