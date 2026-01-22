#pragma once

#include "UI/CodeTextEdit.h"
#include <Editor/Document.h>
#include <LanguageIntelligence/Diagnostic.h>
#include <QPlainTextEdit>
#include <QWidget>

namespace CodeWizard::UI {

class EditorTab final : public QWidget
{
  Q_OBJECT

public:
  explicit EditorTab(Editor::Document *document, QWidget *parent = nullptr);

  [[nodiscard]] QPlainTextEdit *textEdit() const { return m_codeEdit; }
  [[nodiscard]] const Editor::Document *document() const { return m_document; }

signals:
  void modificationChanged(bool modified);
  void diagnosticsReceived(const QList<LanguageIntelligence::Diagnostic> &diagnostics);
  void cursorPositionChanged(Core::Position pos);

private slots:
  void onUndoAvailableChanged();
  void onRedoAvailableChanged();
  void onUserInput(const QString &text, int position);
  void onBackspacePressed(int position);
  void onDeletePressed(int offset);
  void onEnterPressed(int position);
  void onCutRequested(int start, int end);
  void onPasteRequested(int position);
  void onCursorPositionChanged();
private:
  void onDocumentChanged(const Editor::Change& change);
  static int calculateNewCursorPos(int oldPos, size_t removeStart, size_t removeEnd, size_t insertLength);
  void updateCursorPosition();
  CodeTextEdit * m_codeEdit = nullptr;
  Editor::Document* m_document = nullptr;
  Core::Position m_currentCursorPosition;
};

}// namespace CodeWizard::UI