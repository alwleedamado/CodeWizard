// include/UI/MainWindow.h
#pragma once

#include "CommandBar.h"
#include "EditorTab.h"
#include "LanguageIntelligence/Diagnostic.h"
#include "Sidebar.h"
#include "StatusBar.h"
#include <QMainWindow>
#include <QMap>
#include <QMenuBar>
#include <QApplication>

namespace CodeWizard::Editor {
    class Document;
}

namespace CodeWizard::UI {

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void openFile(const QString& filePath);

private slots:
    void onNewFile();
    void onOpenFile();
    void onOpenProject();
    void onCommandSubmitted(const QString& command);
    void onFileOpenedFromSidebar(const QString& filePath);
    void onTabCloseRequested(int index);
    void onModificationChanged(bool modified);
    void onDiagnosticsReceived(const QList<LanguageIntelligence::Diagnostic>& diagnostics);
    void onThemeChanged();
private:
    void setupUi();
    void setupLayout();
    EditorTab* createTab(Editor::Document* doc);
    void updateTabTitle(EditorTab* tab);
    void renderDiagnostics(const QList<LanguageIntelligence::Diagnostic>& diagnostics);
    void openProject(const QString& projectPath);

    QMenuBar* m_menuBar = nullptr;
    CommandBar* m_commandBar = nullptr;
    Sidebar* m_sidebar = nullptr;
    QTabWidget* m_tabWidget = nullptr;
    StatusBar* m_statusBar = nullptr;

    QMap<Editor::Document*, EditorTab*> m_docToTab;
    QString m_currentProjectPath;
    Editor::Document* m_currentDocument = nullptr;
};

} // namespace CodeWizard::UI
