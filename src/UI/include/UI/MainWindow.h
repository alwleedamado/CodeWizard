// include/UI/MainWindow.h
#pragma once

#include "CommandBar.h"
#include "EditorTab.h"
#include "LanguageIntelligence/Diagnostic.h"
#include "Sidebar.h"
#include "StatusBar.h"
#include <QApplication>
#include <QMainWindow>
#include <QMap>
#include <QMenuBar>

namespace CodeWizard::UI {
class Document;
}

namespace CodeWizard::UI {

class MainWindow final : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

  void openFile();

private slots:
  void onNewFile();
  void onOpenFile();
  void onOpenProject();
  void onCommandSubmitted(const QString &command);
  void onFileOpenedFromSidebar(const QString &filePath);
  void onFilePathChanged(const QString &filepath);
  void onCloseTab(int index);
  void updateWindowTitle();
  void onModificationChanged(bool modified);
  void onDiagnosticsReceived(const QList<LanguageIntelligence::Diagnostic> &diagnostics);
  void onThemeChanged();
  void onSaveFileAs();
  void onSaveFile();

private:
  void setupUi();
  void setupLayout();
  void renderDiagnostics(const QList<LanguageIntelligence::Diagnostic> &diagnostics);
  [[nodiscard]] QString getSaveFileName(const QString &suggestedName);
  void openProject(const QString &projectPath);
  static Platform::Path getCurrentWorkingDirectory();
  [[nodiscard]] EditorTab *currentTab() const;
  void onTabChanged(int index);

  QMenuBar *m_menuBar = nullptr;
  CommandBar *m_commandBar = nullptr;
  Sidebar *m_sidebar = nullptr;
  QTabWidget *m_tabWidget = nullptr;
  StatusBar *m_statusBar = nullptr;
  // Actions
  QAction *m_newFileAction = nullptr;
  QAction *m_openFileAction = nullptr;
  QAction *m_saveFileAction = nullptr;
  QAction *m_saveFileAsAction = nullptr;
  QAction *m_closeTabAction = nullptr;
  QString m_currentProjectPath;
};

}// namespace CodeWizard::UI
