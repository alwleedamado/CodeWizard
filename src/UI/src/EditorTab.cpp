#include "UI/EditorTab.h"
#include <QMessageBox>
#include <QFileInfo>
#include "Platform/FileSystem.h"
#include "Core/Logger.h"

using namespace CodeWizard::UI;
using namespace CodeWizard::Platform;

static constexpr std::uint64_t MAX_FILE_SIZE = 15 * 1024 * 1024; // 15MB
static constexpr int MAX_LINE_LENGTH = 1024 * 1024;

EditorTab::EditorTab(QWidget* parent)
    : QWidget(parent), m_editor(new CodeTextEdit(this)) {

    m_layout = new QVBoxLayout();
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
    m_layout->addWidget(m_editor);

    connect(m_editor->document(), &QTextDocument::modificationChanged,
            this, &EditorTab::onModificationChanged);
}

bool EditorTab::loadFromFile(const QString& filepath) {
    Path path(filepath.toStdString());

    // Check file size and long lines using Platform API
    const bool fileTooLarge = isFileTooLarge(path);
    const bool hasLongLines = hasVeryLongLine(path, MAX_LINE_LENGTH);

    if (hasLongLines) {
        QMessageBox::warning(this,
            tr("File with long lines"),
            "This file contains very long lines.\n"
            "Highlighting and editing is disabled for performance.");
    }

    // Use Platform::readFile instead of QFile
    auto result = Platform::readFile(path, Platform::ReadOption::Text);
    if (result.hasError()) {
        QMessageBox::warning(
            this, tr("Error opening file"),
            QString("Cannot read file %1:\n%2.").arg(filepath, QString::fromStdString(result.error().message())));
        return false;
    }

    QString fileContents = QString::fromStdString(result.value());
    m_editor->setDocumentText(fileContents);
    m_editor->setFileName(filepath);

    if (hasLongLines || fileTooLarge) {
        m_editor->document()->setUndoRedoEnabled(false);
    }

    m_editor->document()->setModified(false);
    setFilePath(filepath);
    return true;
}

bool EditorTab::saveToFile(const QString& filepath) {
    Path path(filepath.toStdString());
    QString content = m_editor->documentText();

    // Use Platform::writeFile instead of QFile
    auto result = Platform::writeFile(path, content.toStdString(), Platform::WriteOption::Text);
    if (result.hasError()) {
        QMessageBox::warning(this, "Save Error",
            QString("Cannot write file %1:\n%2.").arg(filepath, QString::fromStdString(result.error().message())));
        return false;
    }

    setFilePath(filepath);
    setModified(false);
    return true;
}

void EditorTab::onModificationChanged(bool modifier) {
    emit modificationChanged(isModified());
}
void EditorTab::onFilePathChanged(const QString &path) {}

bool EditorTab::isFileTooLarge(const Path& filePath) {
  auto result = Platform::readFile(filePath, Platform::ReadOption::Binary);
  return result.hasValue() && result.value().size() > MAX_FILE_SIZE;
}

bool EditorTab::hasVeryLongLine(const Path& filePath, int maxLineLength) {
    auto result = Platform::readFile(filePath, Platform::ReadOption::Binary);
    if (result.hasError()) return false;

    const std::string& content = result.value();
    int currentLineLength = 0;
    bool foundLongLine = false;

    for (const char chr : content) {
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

    if (!foundLongLine && currentLineLength > maxLineLength) {
        foundLongLine = true;
    }

    return foundLongLine;
}
