// src/Editor/Document.cpp
#include "Editor/Document.h"
#include <Core/Logger.h>
#include <Platform/FileSystem.h>

using namespace CodeWizard;

namespace CodeWizard::Editor {

Document::Document(const Platform::Path &filePath) : m_filePath(filePath)
{
  if (!filePath.filename().empty()) {
    auto result = loadFromFile();
    // if (!result.hasValue()) { Core::Logger::error("Failed to load file: {}", filePath.native()); }
  }
}

Result<void> Document::loadFromFile()
{
  if (m_filePath.filename().empty()) { return failure(ErrorCode::InvalidArgument, "No file path"); }

  auto result = Platform::readFile(m_filePath);
  if (!result.hasValue()) { return failure(result.error().code(), result.error().message()); }

  m_buffer.setText(result.value());
  m_isModified = false;
  return success();
}

Result<void> Document::saveToFile() const
{
  if (m_filePath.filename().empty()) { return failure(ErrorCode::InvalidArgument, "No file path"); }

  auto result = Platform::writeFile(m_filePath, m_buffer.text());
  if (!result.hasValue()) { return failure(result.error().code(), result.error().message()); }

  return success();
}

void Document::insertText(Core::Position pos, std::string_view text)
{
  class InsertAction : public Core::UndoableAction
  {
  public:
    InsertAction(Document *doc, Core::Position p, std::string t) : m_doc(doc), m_pos(p), m_text(std::move(t)) {}

    void execute() override { m_doc->rawInsertText(m_pos, m_text); }

    void undo() override
    {
      // Calculate end position after insertion
      size_t startOffset = m_doc->m_buffer.offsetFromPosition(m_pos);
      size_t endOffset = startOffset + m_text.size();
      Core::Position endPos = m_doc->m_buffer.positionFromOffset(endOffset);
      m_doc->rawRemoveText({ m_pos, endPos });
    }

  private:
    Document *m_doc;
    Core::Position m_pos;
    std::string m_text;
  };

  m_undoRedo.execute(std::make_unique<InsertAction>(this, pos, std::string(text)));
}

void Document::removeText(Core::TextRange range)
{
  if (!range.valid()) return;

  // Get exact text being removed
  size_t startOffset = m_buffer.offsetFromPosition(range.start);
  size_t endOffset = m_buffer.offsetFromPosition(range.end);

  // Clamp to buffer bounds
  endOffset = std::min(endOffset, m_buffer.text().size());
  startOffset = std::min(startOffset, endOffset);

  std::string oldText(m_buffer.text().substr(startOffset, endOffset - startOffset));

  class RemoveAction : public Core::UndoableAction {
  public:
    RemoveAction(Document* doc, Core::TextRange r, std::string oldText)
        : m_doc(doc), m_range(r), m_oldText(std::move(oldText)) {}

    void execute() override {
      m_doc->rawRemoveText(m_range);
    }

    void undo() override {
      m_doc->rawInsertText(m_range.start, m_oldText);
    }

  private:
    Document* m_doc;
    Core::TextRange m_range;
    std::string m_oldText;
  };

  m_undoRedo.execute(std::make_unique<RemoveAction>(this, range, std::move(oldText)));

}

void Document::replaceText(std::string_view newText)
{
  class ReplaceAction : public Core::UndoableAction
  {
  public:
    ReplaceAction(Document *doc, std::string oldText, std::string newText)
      : m_doc(doc), m_oldText(std::move(oldText)), m_newText(std::move(newText))
    {}

    void execute() override { m_doc->rawReplaceText(m_newText); }

    void undo() override { m_doc->rawReplaceText(m_oldText); }

  private:
    Document *m_doc;
    std::string m_oldText;
    std::string m_newText;
  };

  std::string oldText = m_buffer.text();
  m_undoRedo.execute(std::make_unique<ReplaceAction>(this, oldText, std::string(newText)));
}

void Document::rawInsertText(Core::Position pos, std::string_view text)
{
  size_t offset = m_buffer.offsetFromPosition(pos);
  m_buffer.insertAt(offset, text);
  Position newEndPos = m_buffer.positionFromOffset(offset + text.size());
  markModified();

  if (m_changeCallback) {
    Change change;
    change.oldRange = { pos, pos };
    change.oldLineEndOffset = offset;
    change.oldLineEndOffset = offset;
    change.oldText = "";
    change.newText = std::string(text);
    change.newRange = {pos, newEndPos};
    change.newLineEndOffset = offset;
    change.newLineEndOffset = offset + text.size();
    m_changeCallback(change);
  }
}

void Document::rawRemoveText(Core::TextRange range)
{
  if (!range.valid()) return;

  size_t start = m_buffer.offsetFromPosition(range.start);
  size_t end = m_buffer.offsetFromPosition(range.end);
  std::string oldText = m_buffer.text().substr(start, end - start);
  m_buffer.removeRange(start, end);

  markModified();

  if (m_changeCallback) {
    Change change;
    change.oldRange = range;
    change.oldText = oldText;
    change.oldLineStartOffset = start;
    change.oldLineEndOffset = end;
    change.newText = "";
    change.newRange = {range.start, range.start};
    change.newLineStartOffset = start;
    change.newLineEndOffset = start;
    m_changeCallback(change);
  }
}

void Document::rawReplaceText(std::string_view newText)
{
  m_buffer.setText(newText);
  markModified();

  if (m_changeCallback) {
    Change change;
    change.oldRange = { { 0, 0 }, m_buffer.positionFromOffset(m_buffer.text().size()) };
    change.oldText = "";// Not used for full replace
    change.newText = std::string(newText);
    m_changeCallback(change);
  }
}

void Document::undo()
{
  if (!m_undoRedo.canUndo()) return;

  // Temporarily disable callback to avoid feedback loop
  auto oldCallback = std::move(m_changeCallback);
  m_changeCallback = nullptr;

  m_undoRedo.undo();

  // Restore callback and trigger update
  m_changeCallback = std::move(oldCallback);
  if (m_changeCallback) {
    Change change;
    change.oldRange = { { 0, 0 }, m_buffer.positionFromOffset(m_buffer.text().size()) };
    change.newText = m_buffer.text();
    m_changeCallback(change);
  }
}

void Document::redo()
{
  if (!m_undoRedo.canRedo()) return;

  auto oldCallback = std::move(m_changeCallback);
  m_changeCallback = nullptr;

  m_undoRedo.redo();

  m_changeCallback = std::move(oldCallback);
  if (m_changeCallback) {
    Change change;
    change.oldRange = { { 0, 0 }, m_buffer.positionFromOffset(m_buffer.text().size()) };
    change.newText = m_buffer.text();
    m_changeCallback(change);
  }
}

}// namespace CodeWizard::Editor