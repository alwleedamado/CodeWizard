#pragma once
#include "Theme/ThemeEngine.h"
#include <Core/EventBus.h>
#include <QInputDialog>
#include <QPainter>
#include <QPlainTextEdit>
#include <QWheelEvent>
using namespace CodeWizard::Core;
// ---------- lock-free events ----------
struct IncChangeEvent
{
  QString file;
  int version;
  int pos;
  int charsRemoved;
  int charsAdded;
};
struct CursorMovedEvent
{
  QString file;
  int pos;
};
struct SelectionEvent
{
  QString file;
  int start;
  int end;
};
template<> struct CodeWizard::Core::EventTraits<IncChangeEvent>
{
  static constexpr bool kThreadSafe = true;
};
template<> struct CodeWizard::Core::EventTraits<CursorMovedEvent>
{
  static constexpr bool kThreadSafe = true;
};
template<> struct CodeWizard::Core::EventTraits<SelectionEvent>
{
  static constexpr bool kThreadSafe = true;
};

/*--------------------------------------------------------------------
 *  CodeTextEdit  –  in-memory code editor (no I/O)
 *------------------------------------------------------------------*/
class CodeTextEdit : public QPlainTextEdit
{
  Q_OBJECT
public:
  explicit CodeTextEdit(QWidget *parent = nullptr);

  // ---- external identity ----
  void setFileName(const QString &name) { m_file = name; }
  QString fileName() const { return m_file; }

  // ---- load / save **text** from outside ----
  void setDocumentText(const QString &text)
  {
    QPlainTextEdit::setPlainText(text);
    m_version = 1;
  }
  QString documentText() const { return QPlainTextEdit::toPlainText(); }

  // ---- zoom ----
  void wheelEvent(QWheelEvent *e) override;
  void zoom(int steps);

  // ---- goto line ----
  void gotoLine();

  // ---- find next ----
  void findNext(const QString &text, bool backwards = false);

  // ---- indent / un-indent ----
  void keyPressEvent(QKeyEvent *e) override;
  void indentBlock(bool unIndent);
  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

protected:
  void resizeEvent(QResizeEvent *e) override;
  void paintEvent(QPaintEvent *e) override;

private slots:
  void onContentsChange(int pos, int charsRemoved, int charsAdded);
  void onCursor();
  void onSelection();

private:
  void updateLineNumberAreaWidth();
  void updateLineNumberArea(const QRect &rect, int dy);
  void applyTheme();

  QWidget *m_lineNumberArea = nullptr;
  QString m_file;
  int m_version = 1;
};

// Line number area widget
class LineNumberArea : public QWidget
{
public:
  explicit LineNumberArea(CodeTextEdit *editor) : QWidget(editor), m_codeEditor(editor) {}

  QSize sizeHint() const override { return QSize(m_codeEditor->lineNumberAreaWidth(), 0); }

protected:
  void paintEvent(QPaintEvent *event) override { m_codeEditor->lineNumberAreaPaintEvent(event); }

private:
  CodeTextEdit *m_codeEditor;
};