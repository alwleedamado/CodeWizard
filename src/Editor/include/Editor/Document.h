// include/Editor/Document.h
#pragma once

#include "Core/Result.h"


#include <Core/UndoRedo.h>
#include <Editor/TextBuffer.h>
#include <Platform/Path.h>
#include <functional>
#include <memory>
#include <string>

namespace CodeWizard::Editor {

struct Change
{
  Core::TextRange oldRange;
  size_t oldLineStartOffset;
  size_t oldLineEndOffset;
  std::string oldText;
  Core::TextRange newRange;
  size_t newLineStartOffset;
  size_t newLineEndOffset;
  std::string newText;
};

class Document
{
public:
  explicit Document(const Platform::Path &filePath = {});
  ~Document() = default;

  // File operations
  [[nodiscard]] Core::Result<void> loadFromFile();
  [[nodiscard]] Core::Result<void> saveToFile() const;

  // Text manipulation (public API - uses undo system)
  void insertText(Core::Position pos, std::string_view text);
  void removeText(Core::TextRange range);
  void replaceText(std::string_view newText);

  // Direct access (for UI sync)
  [[nodiscard]] const TextBuffer &buffer() const { return m_buffer; }
  [[nodiscard]] bool isModified() const noexcept { return m_isModified; }
  [[nodiscard]] bool isUntitled() const noexcept { return m_filePath.filename().empty(); }
  [[nodiscard]] const Platform::Path &filePath() const noexcept { return m_filePath; }

  // Callback for UI updates
  using ChangeCallback = std::function<void(const Change &)>;
  void setChangeCallback(ChangeCallback callback) { m_changeCallback = std::move(callback); }

  // Undo/Redo
  void undo();
  void redo();
  [[nodiscard]] bool canUndo() const noexcept { return m_undoRedo.canUndo(); }
  [[nodiscard]] bool canRedo() const noexcept { return m_undoRedo.canRedo(); }

private:
  // Internal methods (no undo)
  void rawInsertText(Core::Position pos, std::string_view text);
  void rawRemoveText(Core::TextRange range);
  void rawReplaceText(std::string_view newText);
  void markModified() { m_isModified = true; }

  Platform::Path m_filePath;
  TextBuffer m_buffer;
  bool m_isModified = false;
  Core::UndoRedoStack m_undoRedo;
  ChangeCallback m_changeCallback;
};

}// namespace CodeWizard::Editor