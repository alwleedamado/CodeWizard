
#include "EditorWidget.h"

#include "SyntaxHighlighter.h"

#include <QMessageBox>
#include <QTextDocument>
#include <QVBoxLayout>
#include <cstddef>
#include <qfileinfo.h>

static constexpr qint64 MAX_FILE_SIZE = static_cast<const qint64>(15 * 1024 * 1024);
static constexpr int MAX_LINE_LENGTH = 1024 * 1024;

EditorWidget::EditorWidget(QWidget *parent) : QWidget(parent), m_widget(new QPlainTextEdit())
{
  setLayout(new QVBoxLayout());// NOLINT(*-owning-memory)
  setContentsMargins(0, 0, 0, 0);
  layout()->addWidget(m_widget);
  connect(m_widget->document(), &QTextDocument::modificationChanged, this, &EditorWidget::onModificationChanged);
}
bool EditorWidget::loadFromFile(const QString &filepath)
{
  const bool fileTooLarge = EditorWidget::isFileTooLarge(filepath);
  const bool hasLongLines = EditorWidget::hasVeryLongLine(filepath, MAX_LINE_LENGTH);
  if (hasLongLines) {
    QMessageBox::warning(this,
      tr("File with long lines"),
      "This file contains ver long lines"
      "highlighting and editing is disabled for performance");
  }
  QFile file(filepath);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(
      this, tr("Error opening file"), QString("Cannot read file %1:\n%2.").arg(filepath, file.errorString()));
    return false;
  }
  QTextStream stream(&file);
  const QString fileContents = stream.readAll();
  file.close();

  m_widget->setPlainText(fileContents);
  if (hasLongLines || fileTooLarge) {
    m_widget->setLineWrapMode(QPlainTextEdit::NoWrap);
  } else {
    delete m_highlighter;
    m_highlighter = new SyntaxHighlighter(m_widget->document());
    m_widget->setLineWrapMode(QPlainTextEdit::WidgetWidth);
  }
  m_widget->document()->setModified(false);
  setFilePath(filepath);
  return true;
}
bool EditorWidget::saveToFile(const QString &filepath)
{
  QFile file(filepath);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, "Save Error", QString("Cannot write file %1:\n%2.").arg(filepath, file.errorString()));
    return false;
  }
  QTextStream stream(&file);
  stream.setEncoding(QStringConverter::Utf8);
  stream << m_widget->toPlainText();
  setFilePath(filepath);
  file.close();
  setModified(false);
  return true;
}

void EditorWidget::onModificationChanged() { emit modificationChanged(isModified()); }

bool EditorWidget::isFileTooLarge(const QString &filePath) { return QFileInfo(filePath).size() > MAX_FILE_SIZE; }
bool EditorWidget::hasVeryLongLine(const QString &filePath, int maxLineLength)
{
  QFile file(filePath);
  if (!file.open(QFile::ReadOnly | QFile::Text)) { return false; }
  int currentLineLength = 0;
  bool foundLongLine = false;
  while (!file.atEnd() && currentLineLength < maxLineLength) {
    for (QByteArray const buffer = file.read(maxLineLength); char const chr : buffer) {
      if (chr == '\n' || chr == '\r') {
        if (currentLineLength > maxLineLength) {
          foundLongLine = true;
          break;
        }
        currentLineLength = 0;
      } else {
        currentLineLength++;
      }
    }
  }
  if (!foundLongLine && currentLineLength > maxLineLength) { foundLongLine = true; }
  file.close();
  return foundLongLine;
}
