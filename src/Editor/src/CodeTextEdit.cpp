#include "Editor/CodeTextEdit.h"
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QFontDatabase>
#include <QPainter>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <limits>

class LineNumberArea;
namespace CodeWizard::Editor {
CodeTextEdit::CodeTextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
  // Apply theme styling
  applyTheme();

  setLineWrapMode(QPlainTextEdit::NoWrap);

  // Create line number area
  m_lineNumberArea = new LineNumberArea(this);

  // Connect signals
  connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeTextEdit::onCursor);
  connect(this, &QPlainTextEdit::selectionChanged, this, &CodeTextEdit::onSelection);
  connect(document(), &QTextDocument::contentsChange, this, &CodeTextEdit::onContentsChange);
  connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeTextEdit::updateLineNumberAreaWidth);
  connect(this, &QPlainTextEdit::updateRequest, this, &CodeTextEdit::updateLineNumberArea);
  // Initial setup
  updateLineNumberAreaWidth();
  m_highlighter = std::make_unique<CodeWizard::Editor::TreeSitterHighlighter>(document());
}

void CodeTextEdit::setLanguage(const std::string &languageId)
{
  if (m_highlighter) { m_highlighter->setLanguage("cpp"); }
}

void CodeTextEdit::applyTheme()
{
  const auto &theme  = CodeWizard::Theme::ThemeEngine::instance();
  const auto &fonts  = theme.fonts();
  const auto &colors = theme.colors();

  // Set code font
  setFont(fonts.codeFont);

  // Set background and text colors
  QPalette palette;
  palette.setColor(QPalette::Base, colors.editorBackground);
  palette.setColor(QPalette::Text, colors.editorForeground);
  palette.setColor(QPalette::Highlight, colors.selectionBackground);
  palette.setColor(QPalette::HighlightedText, colors.selectionForeground);
  setPalette(palette);
}

int CodeTextEdit::lineNumberAreaWidth()
{
  const auto &theme = CodeWizard::Theme::ThemeEngine::instance();
  QFont font        = theme.fonts().codeFont;
  font.setPointSize(qMax(8, font.pointSize() - 1));

  int digits = 1;
  int max    = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  QFontMetrics fm(font);
  int space = 10 + fm.horizontalAdvance(QLatin1Char('9')) * digits;
  return space;
}

void CodeTextEdit::updateLineNumberAreaWidth() { setViewportMargins(lineNumberAreaWidth(), 0, 0, 0); }

void CodeTextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
  if (dy != 0) {
    m_lineNumberArea->scroll(0, dy);
  } else {
    m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());
  }

  if (rect.contains(viewport()->rect())) { updateLineNumberAreaWidth(); }
}

void CodeTextEdit::resizeEvent(QResizeEvent *e)
{
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeTextEdit::paintEvent(QPaintEvent *e) { QPlainTextEdit::paintEvent(e); }

void CodeTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
  QPainter painter(m_lineNumberArea);

  const auto &colors = CodeWizard::Theme::ThemeEngine::instance().colors();
  const auto &fonts  = CodeWizard::Theme::ThemeEngine::instance().fonts();

  // Background
  painter.fillRect(event->rect(), colors.lineNumberBackground);

  // Font
  QFont font = fonts.codeFont;
  font.setPointSize(qMax(8, font.pointSize() - 1));
  painter.setFont(font);
  painter.setPen(colors.lineNumberForeground);

  // Get first visible block
  QTextBlock block = firstVisibleBlock();
  int blockNumber  = block.blockNumber();
  int top          = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom       = top + static_cast<int>(blockBoundingRect(block).height());

  // Paint line numbers
  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.drawText(0, top, m_lineNumberArea->width() - 4, fontMetrics().height(), Qt::AlignRight, number);
    }

    block = block.next();
    ++blockNumber;
    top    = bottom;
    bottom = top + static_cast<int>(blockBoundingRect(block).height());
  }
}

void CodeTextEdit::wheelEvent(QWheelEvent *e)
{
  if (e->modifiers() & Qt::ControlModifier) {
    int delta = e->angleDelta().y();
    zoom(delta > 0 ? 1 : -1);
    e->accept();
    return;
  }
  QPlainTextEdit::wheelEvent(e);
}

void CodeTextEdit::zoom(int steps)
{
  QFont f  = font();
  int size = f.pointSize() + steps;
  if (size < 6 || size > 40) return;
  f.setPointSize(size);
  setFont(f);
  // Update line number area after font change
  updateLineNumberAreaWidth();
}

void CodeTextEdit::gotoLine()
{
  bool ok;
  int line = QInputDialog::getInt(this,
    tr("Go to Line"),
    tr("Line number:"),
    textCursor().blockNumber() + 1,
    1,
    std::numeric_limits<int>::max(),
    1,
    &ok);
  if (!ok) return;
  QTextCursor c(document());
  c.movePosition(QTextCursor::Start);
  c.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line - 1);
  setTextCursor(c);
}

void CodeTextEdit::findNext(const QString &text, bool backwards)
{
  if (text.isEmpty()) return;

  QTextDocument::FindFlags flags;
  if (backwards) flags |= QTextDocument::FindBackward;
  QTextCursor c = document()->find(text, textCursor(), flags);
  if (!c.isNull()) {
    setTextCursor(c);
  } else {
    // Wrap around to beginning/end
    c = QTextCursor(document());
    if (backwards) { c.movePosition(QTextCursor::End); }
    c = document()->find(text, c, flags);
    if (!c.isNull()) { setTextCursor(c); }
  }
}

void CodeTextEdit::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Tab && !e->text().isEmpty()) {
    indentBlock(e->modifiers() & Qt::ShiftModifier);
    e->accept();
    return;
  }
  if (e->key() == Qt::Key_G && e->modifiers() & Qt::ControlModifier) { gotoLine(); }
  // if (e->key() == Qt::Key_F && e->modifiers() & Qt::ControlModifier) { findNext(); }

  QPlainTextEdit::keyPressEvent(e);
}

void CodeTextEdit::indentBlock(bool unIndent)
{
  QTextCursor c = textCursor();
  c.beginEditBlock();
  if (c.hasSelection()) {
    int b = c.selectionStart(), e = c.selectionEnd();
    c.setPosition(b);
    do {
      if (unIndent) {
        c.movePosition(QTextCursor::StartOfLine);
        int spacesToRemove = std::min(4, c.positionInBlock());
        c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, spacesToRemove);
        if (c.selectedText().trimmed().isEmpty()) { c.removeSelectedText(); }
      } else {
        c.insertText("    ");
      }
      c.movePosition(QTextCursor::Down);
    } while (c.position() <= e && !c.atEnd());
  } else {
    if (unIndent) {
      c.movePosition(QTextCursor::StartOfLine);
      int spacesToRemove = std::min(4, c.positionInBlock());
      c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, spacesToRemove);
      if (c.selectedText().trimmed().isEmpty()) { c.removeSelectedText(); }
    } else {
      c.insertText("    ");
    }
  }
  c.endEditBlock();
}

void CodeTextEdit::onContentsChange(int pos, int charsRemoved, int charsAdded)
{
  IncChangeEvent ev;
  ev.file         = m_file;
  ev.version      = m_version++;
  ev.pos          = pos;
  ev.charsRemoved = charsRemoved;
  ev.charsAdded   = charsAdded;

  postEvent(std::move(ev));
}

void CodeTextEdit::onCursor()
{
  CursorMovedEvent ev{ m_file, textCursor().position() };
  postEvent(std::move(ev));
}

void CodeTextEdit::onSelection()
{
  QTextCursor c = textCursor();
  SelectionEvent ev{ m_file, c.selectionStart(), c.selectionEnd() };
  postEvent(std::move(ev));
}
}// namespace CodeWizard::Editor