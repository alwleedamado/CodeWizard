//
// Created by cyberbit on 1/13/26.
//

#include "EditorWidget.h"

#include "../SyntaxHighlighter.h"

#include <QFile>
#include <QMessageBox>
#include <QTextDocument>
#include <QVBoxLayout>

EditorWidget::EditorWidget(QWidget* parent)
  : QWidget(parent), m_widget(new QPlainTextEdit())
{
  setLayout(new QVBoxLayout());
  setContentsMargins(0, 0, 0, 0);
  layout()->addWidget(m_widget);
  new SyntaxHighlighter(m_widget->document());
  connect(m_widget->document(), &QTextDocument::modificationChanged, this, &EditorWidget::onModificationChanged);
}
EditorWidget::~EditorWidget()
{
  delete m_widget;
}
bool EditorWidget::loadFromFile(const QString &filepath)
{
  QFile file(filepath);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Error opening file"),
    QString("Cannot read file %1:\n%2.")
              .arg(filepath, file.errorString()));
    return false;
  }
  QTextStream stream(&file);
  m_widget->setPlainText(stream.readAll());
  m_widget->document()->setModified(false);
  setFilePath(filepath);
  file.close();
  return true;
}
bool EditorWidget::saveToFile(const QString &filepath)
{
  QFile file(filepath);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, "Save Error",
        QString("Cannot write file %1:\n%2.")
            .arg(filepath, file.errorString()));
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

void EditorWidget::onModificationChanged()
{
  emit modificationChanged(isModified());
}