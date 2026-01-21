// include/Editor/Document.h
#pragma once

#include "Editor/TextBuffer.h"
#include <Core/Result.h>
#include <Core/Types.h>
#include <Core/Undo.h>
#include <Platform/Path.h>
#include <functional>
#include <memory>
#include <vector>
using namespace CodeWizard::Core;
namespace CodeWizard::Editor {

struct Change
{
  Core::TextRange range;
  std::string oldText;
  std::string newText;
};

class Document
{
public:
  Document();
  explicit Document(const Platform::Path &filePath);

  // File operations
  [[nodiscard]] Result<void> loadFromFile();
  [[nodiscard]] Result<void> saveToFile();
  [[nodiscard]] Result<void> saveAs(const Platform::Path &newPath);

  // Content access
  [[nodiscard]] const TextBuffer &buffer() const noexcept { return m_buffer; }
  [[nodiscard]] const Platform::Path &filePath() const noexcept { return m_filePath; }
  [[nodiscard]] bool isModified() const noexcept { return m_isModified; }
  [[nodiscard]] bool isUntitled() const noexcept { return m_filePath.native().empty(); }

  // Editing
  void insertText(Core::Position pos, std::string_view text);
  void removeText(Core::TextRange range);
  void updateText(const std::string& newText);

  // Undo/Redo
  void undo();
  void redo();
  [[nodiscard]] bool canUndo() const noexcept;
  [[nodiscard]] bool canRedo() const noexcept;
  void rawInsertText(Position start, const std::string &string);
  void rawRemoveText(TextRange range);
  // Events
  using ChangeCallback = std::function<void(const Change &)>;
  void setChangeCallback(ChangeCallback callback);

private:
  void markModified();

  Platform::Path m_filePath;
  TextBuffer m_buffer;
  bool m_isModified = false;
  ChangeCallback m_changeCallback;

  UndoRedoStack m_undoRedoStack;
};

}// namespace CodeWizard::Editor
