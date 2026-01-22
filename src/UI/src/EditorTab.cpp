#include "UI/EditorTab.h"
#include <Editor/Document.h>
#include <QApplication>
#include <QClipboard>
#include <QScrollArea>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QVBoxLayout>
#include <Theme/ThemeEngine.h>

using namespace CodeWizard;

namespace CodeWizard::UI {

EditorTab::EditorTab(Editor::Document *document, QWidget *parent) : QWidget(parent), m_codeEdit(new CodeTextEdit(this)), m_document(document)
{
  m_codeEdit->setPlainText(QString::fromStdString(document->buffer().text()));
  // Apply theme
  const auto &colors = Theme::ThemeEngine::instance().colors();
  const auto &fonts = Theme::ThemeEngine::instance().fonts();
  m_codeEdit->setFont(fonts.codeFont);
  m_codeEdit->setStyleSheet(
    QString("QPlainTextEdit {"
            "   background: %1;"
            "   color: %2;"
            "   selection-background-color: %3;"
            "   padding: 8px;"
            "}")
      .arg(colors.editorBackground.name())
      .arg(colors.editorForeground.name())
      .arg(colors.editorSelection.name()));

  // Layout
  auto *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_codeEdit);
  setLayout(layout);
  document->setChangeCallback([this](const Editor::Change &change) { onDocumentChanged(change); });
  connect(m_codeEdit, &CodeTextEdit::undoAvailable, this, &EditorTab::onUndoAvailableChanged);
  connect(m_codeEdit, &CodeTextEdit::redoAvailable, this, &EditorTab::onRedoAvailableChanged);
  connect(m_codeEdit, &CodeTextEdit::userInput, this, &EditorTab::onUserInput);
  connect(m_codeEdit, &CodeTextEdit::backspacePressed, this, &EditorTab::onBackspacePressed);
  connect(m_codeEdit, &CodeTextEdit::deletePressed, this, &EditorTab::onDeletePressed);
  connect(m_codeEdit, &CodeTextEdit::enterPressed, this, &EditorTab::onEnterPressed);
  connect(m_codeEdit, &CodeTextEdit::cutRequested, this, &EditorTab::onCutRequested);
  connect(m_codeEdit, &CodeTextEdit::pasteRequested, this, &EditorTab::onPasteRequested);
  connect(m_codeEdit, &CodeTextEdit::cursorPositionChanged, this, &EditorTab::onCursorPositionChanged);
}

void EditorTab::onUndoAvailableChanged()
{
  // Could update UI (e.g., disable undo button)
}

void EditorTab::onRedoAvailableChanged()
{
  // Could update UI (e.g., disable redo button)
}
void EditorTab::onUserInput(const QString &text, int position)
{
  const Core::Position pos = m_document->buffer().positionFromOffset(position);
  m_document->insertText(pos, text.toStdString());
}
void EditorTab::onBackspacePressed(int position)
{
  if (m_currentCursorPosition.line == 0 && m_currentCursorPosition.character == 0) {
    return;// Beginning of document
  }

  Core::Position deleteEnd = m_currentCursorPosition;
  Core::Position deleteStart = deleteEnd;

  if (deleteEnd.character > 0) {
    deleteStart.character--;
  } else {
    // Move to end of previous line
    deleteStart.line--;
    std::string_view prevLine = m_document->buffer().lineView(deleteStart.line);
    deleteStart.character = static_cast<uint32_t>(prevLine.length());
  }

  m_document->removeText({ deleteStart, deleteEnd });
}
void EditorTab::onDeletePressed(int offset)
{
  std::string_view currentLine = m_document->buffer().lineView(m_currentCursorPosition.line);

  if (m_currentCursorPosition.character >= currentLine.length()) {
    // At end of line - check if there's a next line
    if (m_currentCursorPosition.line + 1 >= m_document->buffer().lineCount()) {
      return;// End of document
    }

    // Delete newline character
    Core::Position deleteStart = m_currentCursorPosition;
    Core::Position deleteEnd = { m_currentCursorPosition.line + 1, 0 };
    m_document->removeText({ deleteStart, deleteEnd });
  } else {
    // Delete character within line
    Core::Position deleteStart = m_currentCursorPosition;
    Core::Position deleteEnd = deleteStart;
    deleteEnd.character++;
    m_document->removeText({ deleteStart, deleteEnd });
  }
}
void EditorTab::onEnterPressed(int offset)
{
  Core::Position pos = m_document->buffer().positionFromOffset(offset);
  m_document->insertText(pos, "\n");
}
void EditorTab::onCutRequested(int start, int end)
{
  Core::Position posStart = m_document->buffer().positionFromOffset(start);
  Core::Position posEnd = m_document->buffer().positionFromOffset(end);
  m_document->removeText({ posStart, posEnd });
}
void EditorTab::onPasteRequested(int position) {}
void EditorTab::onCursorPositionChanged() { updateCursorPosition(); }
void EditorTab::onDocumentChanged(const Editor::Change &change)
{
  QTextCursor cursor = m_codeEdit->textCursor();
  QTextDocument *doc = m_codeEdit->document();

  // Save cursor position BEFORE any changes
  int savedCursorPos = cursor.position();
  int scrollPos = m_codeEdit->verticalScrollBar()->value();
  bool wasAtBottom = (scrollPos >= m_codeEdit->verticalScrollBar()->maximum() - 10);

  if (change.newText.empty()) {
    cursor.setPosition(static_cast<int>(change.oldLineStartOffset));
    cursor.setPosition(static_cast<int>(change.oldLineEndOffset), QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
  } else {
    cursor.insertText(QString::fromStdString(change.newText));
  }

  // Calculate new cursor position
  int newCursorPos = change.newLineEndOffset;
  int maxPos = doc->characterCount();
  newCursorPos = std::max(0, std::min(newCursorPos, maxPos));

  // Set cursor AFTER the inserted text
  cursor.setPosition(newCursorPos);
  m_codeEdit->setTextCursor(cursor);

  // Update internal cursor position
  updateCursorPosition();

  // Restore scroll
  if (wasAtBottom) {
    m_codeEdit->verticalScrollBar()->setValue(m_codeEdit->verticalScrollBar()->maximum());
  } else {
    m_codeEdit->verticalScrollBar()->setValue(scrollPos);
  }
}

int EditorTab::calculateNewCursorPos(int oldPos, size_t removeStart, size_t removeEnd, size_t insertLength)
{
  int intRemoveStart = static_cast<int>(removeStart);
  int intRemoveEnd = static_cast<int>(removeEnd);
  int intInsertLength = static_cast<int>(insertLength);

  if (oldPos < intRemoveStart) {
    // Before change region - no adjustment needed
    return oldPos;
  } else if (oldPos <= intRemoveEnd) {
    // Inside removed region - move to start of inserted text
    return intRemoveStart + intInsertLength;
  } else {
    // After change region - adjust by net change
    int netChange = intInsertLength - (intRemoveEnd - intRemoveStart);
    return oldPos + netChange;
  }
}

void EditorTab::updateCursorPosition()
{
  const QTextCursor cursor = m_codeEdit->textCursor();
  const Core::Position pos{ static_cast<uint32_t>(cursor.blockNumber()),
    static_cast<uint32_t>(cursor.positionInBlock()) };
  m_currentCursorPosition = pos;
  emit cursorPositionChanged(pos);
}
}// namespace CodeWizard::UI