// src/Document.cpp
#include "Editor/Document.h"
#include <Platform/FileSystem.h>
#include <Services/GlobalServices.h>
#include <algorithm>

namespace CodeWizard::Editor {
class RemoveAction final : public Core::UndoableAction
{
public:
  RemoveAction(Document *doc, Core::TextRange r) : m_doc(doc), m_range(r) {}

  void execute() override
  {
    m_oldText = m_doc->buffer().text().substr(m_doc->buffer().offsetFromPosition(m_range.start),
    m_doc->buffer().offsetFromPosition(m_range.end) - m_doc->buffer().offsetFromPosition(m_range.start));
    m_doc->rawRemoveText(m_range);
  }

  void undo() override { m_doc->rawInsertText(m_range.start, m_oldText); }

private:
  Document *m_doc;
  Core::TextRange m_range;
  std::string m_oldText;
};

class InsertAction final : public Core::UndoableAction
{
public:
  InsertAction(Document *doc, Core::Position p, std::string t) : m_doc(doc), m_pos(p), m_text(std::move(t)) {}

  void execute() override
  {
    m_endPos = TextBuffer::computeEndPosition(m_pos, m_text);
    m_doc->rawInsertText(m_pos, m_text);
  }

  void undo() override { m_doc->rawRemoveText({ m_pos, m_endPos }); }

private:
  Document *m_doc;
  Position m_pos;
  Position m_endPos;
  std::string m_text;
};

Document::Document() = default;

Document::Document(const Platform::Path &filePath) : m_filePath(filePath) {}

Result<void> Document::loadFromFile()
{
  if (m_filePath.native().empty()) { return failure(Core::ErrorCode::InvalidArgument, "No file path set"); }

  auto result = Platform::readFile(m_filePath);
  if (!result.hasValue()) { return failure(result.error().code(), result.error().message()); }

  m_buffer = TextBuffer(result.value());
  m_isModified = false;
  m_undoRedoStack.clear();
  return success();
}

Result<void> Document::saveToFile()
{
  if (m_filePath.native().empty()) { return failure(Core::ErrorCode::InvalidArgument, "No file path set"); }

  auto result = Platform::writeFile(m_filePath, m_buffer.text());
  if (!result.hasValue()) { return failure(result.error().code(), result.error().message()); }

  m_isModified = false;
  return success();
}

Result<void> Document::saveAs(const Platform::Path &newPath)
{
  auto result = Platform::writeFile(newPath, m_buffer.text());
  if (!result.hasValue()) { return failure(result.error().code(), result.error().message()); }

  m_filePath = newPath;
  m_isModified = false;
  return success();
}

void Document::insertText(Core::Position pos, std::string_view text)
{
  m_undoRedoStack.execute(std::make_unique<InsertAction>(this, pos, std::string(text)));
}

void Document::removeText(Core::TextRange range)
{
  m_undoRedoStack.execute(std::make_unique<RemoveAction>(this, range));
}

void Document::markModified() { m_isModified = true; }

void Document::undo() { m_undoRedoStack.undo(); }

void Document::redo() { m_undoRedoStack.redo(); }

bool Document::canUndo() const noexcept { return m_undoRedoStack.canUndo(); }

bool Document::canRedo() const noexcept { return m_undoRedoStack.canRedo(); }

void Document::rawInsertText(Position start, const std::string &string)
{
  m_buffer.insertText(start, string);
  m_isModified = true;
}

void Document::rawRemoveText(TextRange range)
{
  m_buffer.removeText(range);
  m_isModified = true;
}

void Document::setChangeCallback(ChangeCallback callback) { m_changeCallback = std::move(callback); }

}// namespace CodeWizard::Editor
