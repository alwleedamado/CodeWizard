#include "UI/CodeTextEdit.h"
#include <QApplication>
#include <QKeySequence>

namespace CodeWizard::UI {

CodeTextEdit::CodeTextEdit(QWidget* parent) : QPlainTextEdit(parent) {
  setLineWrapMode(NoWrap);
}

void CodeTextEdit::keyPressEvent(QKeyEvent* event) {
  // Handle standard shortcuts
  if (event == QKeySequence::Copy) {
    copy();
    return;
  }
  if (event == QKeySequence::Cut) {
    int const start = textCursor().selectionStart();
    int const end = textCursor().selectionEnd();
    emit cutRequested(start, end);
    return;
  }
  if (event == QKeySequence::Paste) {
    emit pasteRequested(getCurrentPosition());
    return;
  }

  // Handle special keys
  if (event->key() == Qt::Key_Backspace) {
    const int a = getCurrentPosition();
    emit backspacePressed(getCurrentPosition());
    return;
  }
  if (event->key() == Qt::Key_Delete) {
    emit deletePressed(getCurrentPosition());
    return;
  }
  if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
    emit enterPressed(getCurrentPosition());
    return;
  }

  // Handle text input
  if (QString const text = event->text(); !text.isEmpty()) {
    emit userInput(text, getCurrentPosition());
    return;
  }

  // Let Qt handle navigation keys
  QPlainTextEdit::keyPressEvent(event);
}

int CodeTextEdit::getCurrentPosition() const {
  return textCursor().position();
}

} // namespace CodeWizard::UI