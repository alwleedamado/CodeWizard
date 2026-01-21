// include/UI/EditorTab.h
#pragma once

#include "LanguageIntelligence/Diagnostic.h"


#include <QList>
#include <QPlainTextEdit>
#include <QWidget>

namespace CodeWizard::Editor {
    class Document;
}

namespace CodeWizard::UI {

class EditorTab : public QWidget {
    Q_OBJECT

public:
    explicit EditorTab(CodeWizard::Editor::Document* document, QWidget* parent = nullptr);
    ~EditorTab() override;

    [[nodiscard]] QPlainTextEdit* textEdit() const { return m_textEdit; }
    [[nodiscard]] const Editor::Document* document() const { return m_document; }

signals:
    void modificationChanged(bool modified);
    void diagnosticsReceived(const QList<LanguageIntelligence::Diagnostic>& diagnostics);

private slots:
    void onTextChanged();
    void onCursorPositionChanged();

private:
    void connectDocument();
    void applyDiagnostics(const QList<LanguageIntelligence::Diagnostic>& diagnostics);
    void clearSquiggles();
    void renderSquiggle(int startPos, int endPos, const QColor& color);

    Editor::Document* m_document = nullptr;
    QPlainTextEdit* m_textEdit = nullptr;
    QList<QTextEdit::ExtraSelection> m_squiggles;
};

} // namespace CodeWizard::UI
