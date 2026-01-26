#pragma once

#include "CodeTextEdit.h"


#include "Platform/Path.h"
#include <QVBoxLayout>
#include <QWidget>

namespace CodeWizard::UI {

class EditorTab final : public QWidget {
    Q_OBJECT

public:
    explicit EditorTab(QWidget* parent = nullptr);
    ~EditorTab() override = default;

    // File operations
    bool loadFromFile(const QString& filepath);
    bool saveToFile(const QString& filepath);

    [[nodiscard]] bool isModified() const { return m_editor->document()->isModified(); }
    void setModified(bool modified) { m_editor->document()->setModified(modified); }
    [[nodiscard]] QString getFilePath() const { return m_filePath; }
    void setFilePath(const QString& filepath) { m_filePath = filepath; }

    [[nodiscard]] CodeTextEdit* getEditor() const { return m_editor; }

signals:
    void modificationChanged(bool modified);
    void filePathChanged(const QString& filepath);

private slots:
    void onModificationChanged(bool modified);
    void onFilePathChanged(const QString& path);
private:
    static bool isFileTooLarge(const CodeWizard::Platform::Path& filePath);
    static bool hasVeryLongLine(const CodeWizard::Platform::Path& filePath, int maxLineLength = 100000);

    CodeTextEdit* m_editor = nullptr;
    QString m_filePath;
    QVBoxLayout* m_layout = nullptr;
};

} // namespace CodeWizard::UI
