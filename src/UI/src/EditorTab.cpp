// src/EditorTab.cpp
#include "UI/EditorTab.h"

#include "Theme/ThemeEngine.h"
#include <Editor/Document.h>
#include <QFontDatabase>
#include <QTextBlock>
#include <QTextCursor>
#include <QVBoxLayout>

using namespace CodeWizard;

namespace CodeWizard::UI {

EditorTab::EditorTab(Editor::Document *document, QWidget *parent) : QWidget(parent), m_document(document)
{
  m_textEdit = new QPlainTextEdit(this);
  m_textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
  const auto &colors = Theme::ThemeEngine::instance().colors();
  const auto &fonts = Theme::ThemeEngine::instance().fonts();

  m_textEdit->setFont(fonts.codeFont);
  m_textEdit->setStyleSheet(
    QString("QPlainTextEdit {"
            "   background: %1;"
            "   color: %2;"
            "   selection-background-color: %3;"
            "   padding: 8px;"
            "}")
      .arg(colors.editorBackground.name())
      .arg(colors.editorForeground.name())
      .arg(colors.editorSelection.name()));

  auto *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_textEdit);
  setLayout(layout);

  // Load content
  m_textEdit->setPlainText(QString::fromStdString(m_document->buffer().text()));

  connectDocument();
  connect(m_textEdit, &QPlainTextEdit::textChanged, this, &EditorTab::onTextChanged);
  connect(m_textEdit, &QPlainTextEdit::cursorPositionChanged, this, &EditorTab::onCursorPositionChanged);
}

EditorTab::~EditorTab() = default;

void EditorTab::connectDocument()
{
  // For LSP sync (future)
}

void EditorTab::onTextChanged()
{
  const std::string text = m_textEdit->toPlainText().toStdString();
  m_document->updateText(text);
}

void EditorTab::onCursorPositionChanged()
{
  // Future: send cursor pos to LSP for hover
}

void EditorTab::applyDiagnostics(const QList<LanguageIntelligence::Diagnostic> &diagnostics)
{
  clearSquiggles();
  QTextDocument *doc = m_textEdit->document();

  for (const auto &diag : diagnostics) {
    // Convert Core::Position to QTextCursor positions
    QTextBlock startBlock = doc->findBlockByNumber(diag.range.start.line);
    QTextBlock endBlock = doc->findBlockByNumber(diag.range.end.line);

    int startPos = startBlock.position() + diag.range.start.character;
    int endPos = endBlock.position() + diag.range.end.character;

    // Clamp to document bounds
    startPos = qMax(0, qMin(startPos, doc->characterCount() - 1));
    endPos = qMax(startPos, qMin(endPos, doc->characterCount() - 1));

    QColor color;
    if (diag.severity == LanguageIntelligence::DiagnosticSeverity::Error) {
      color = Qt::red;
    } else {
      color = QColor(255, 255, 200);// Light yellow for warnings
    }

    renderSquiggle(startPos, endPos, color);
  }

  m_textEdit->setExtraSelections(m_squiggles);
  emit diagnosticsReceived(diagnostics);
}

void EditorTab::clearSquiggles() { m_squiggles.clear(); }

void EditorTab::renderSquiggle(int startPos, int endPos, const QColor &color)
{
  QTextEdit::ExtraSelection selection;
  selection.cursor = QTextCursor(m_textEdit->document());
  selection.cursor.setPosition(startPos);
  selection.cursor.setPosition(endPos, QTextCursor::KeepAnchor);

  QTextCharFormat format;
  format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
  format.setUnderlineColor(color);
  selection.format = format;

  m_squiggles.append(selection);
}

}// namespace CodeWizard::UI
