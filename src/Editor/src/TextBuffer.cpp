#include "Editor/TextBuffer.h"
#include <algorithm>
#include <stdexcept>

using namespace CodeWizard;

namespace CodeWizard::Editor {

TextBuffer::TextBuffer(std::string text) : m_text(std::move(text)) { updateLineEnds(); }

std::string_view TextBuffer::lineView(uint32_t lineIndex) const
{
  if (!m_lineCacheValid) { updateLineEnds(); }

  if (lineIndex >= m_lineEnds.size()) { return {}; }

  size_t start = (lineIndex == 0) ? 0 : m_lineEnds[lineIndex - 1] + 1;
  size_t end = m_lineEnds[lineIndex];

  // Handle case where text doesn't end with newline
  if (end > m_text.size()) { end = m_text.size(); }

  return std::string_view(m_text).substr(start, end - start);
}

uint32_t TextBuffer::lineCount() const noexcept
{
  if (!m_lineCacheValid) { const_cast<TextBuffer *>(this)->updateLineEnds(); }
  return static_cast<uint32_t>(m_lineEnds.size());
}

Core::Position TextBuffer::positionFromOffset(size_t offset) const
{
  if (offset > m_text.size()) { throw std::out_of_range("Offset out of range"); }

  if (m_text.empty()) { return { 0, 0 }; }

  if (!m_lineCacheValid) { updateLineEnds(); }

  // Find line containing offset
  auto it = std::upper_bound(m_lineEnds.begin(), m_lineEnds.end(), offset);
  uint32_t line = static_cast<uint32_t>(it - m_lineEnds.begin());

  size_t lineStart;
  if (line == 0) {
    lineStart = 0;
  } else {
    lineStart = m_lineEnds[line - 1] + 1;
  }

  // Character is offset from line start (excluding newline)
  uint32_t character = static_cast<uint32_t>(offset - lineStart);
  return { line, character };
}

size_t TextBuffer::offsetFromPosition(Core::Position pos) const
{
  if (!m_lineCacheValid) { const_cast<TextBuffer *>(this)->updateLineEnds(); }

  if (m_text.empty()) { return 0; }

  // Clamp line to valid range
  uint32_t clampedLine = std::min(pos.line, static_cast<uint32_t>(m_lineEnds.size()));

  size_t lineStart;
  if (clampedLine == 0) {
    lineStart = 0;
  } else {
    if (clampedLine - 1 < m_lineEnds.size()) {
      lineStart = m_lineEnds[clampedLine - 1] + 1;
    } else {
      lineStart = m_text.size();
    }
  }

  // Get actual line content to validate character position
  std::string_view lineContent = lineView(clampedLine);
  uint32_t clampedChar = std::min(pos.character, static_cast<uint32_t>(lineContent.length()));

  size_t offset = lineStart + clampedChar;
  return std::min(offset, m_text.size());
}

void TextBuffer::insertAt(size_t offset, std::string_view text)
{
  if (offset > m_text.size()) { throw std::out_of_range("Insert offset out of range"); }

  m_text.insert(offset, text);
  invalidateLineCache();
}

void TextBuffer::removeRange(size_t startOffset, size_t endOffset)
{
  if (startOffset > endOffset || endOffset > m_text.size()) { throw std::out_of_range("Invalid range"); }

  if (startOffset == endOffset) { return; }

  m_text.erase(startOffset, endOffset - startOffset);
  invalidateLineCache();
}

void TextBuffer::setText(std::string_view text)
{
  m_text.assign(text);
  invalidateLineCache();
}

std::string_view TextBuffer::getLine(uint32_t line) const { return lineView(line); }

size_t TextBuffer::getLineStartOffset(uint32_t line) const
{
  if (!m_lineCacheValid) { updateLineEnds(); }

  if (line == 0) { return 0; }

  if (line - 1 >= m_lineEnds.size()) { return m_text.size(); }

  return m_lineEnds[line - 1] + 1;
}

size_t TextBuffer::getLineEndOffset(uint32_t line) const
{
  if (!m_lineCacheValid) { updateLineEnds(); }

  if (line >= m_lineEnds.size()) { return m_text.size(); }

  return m_lineEnds[line];
}

void TextBuffer::updateLineEnds() const
{
  m_lineEnds.clear();

  if (m_text.empty()) {
    m_lineCacheValid = true;
    return;
  }

  size_t pos = 0;
  while ((pos = m_text.find('\n', pos)) != std::string::npos) {
    m_lineEnds.push_back(pos);
    pos++;
  }

  // Always add end of buffer as final line boundary
  // This ensures line count = m_lineEnds.size()
  if (m_text.back() != '\n') { m_lineEnds.push_back(m_text.size()); }

  m_lineCacheValid = true;
}

void TextBuffer::invalidateLineCache() const { m_lineCacheValid = false; }

}// namespace CodeWizard::Editor