#include "UI/MainWindow.h"

#include "Platform/FileSystem.h"
#include "Theme/ThemeEngine.h"
#include "UI/CommandBar.h"
#include "UI/ProjectView.h"
#include "UI/Sidebar.h"
#include "UI/StatusBar.h"
#include <Platform/Path.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QVBoxLayout>
#include <Editor/TreeSitterHighlighter.h>

using namespace CodeWizard;

namespace CodeWizard::UI {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  setupUi();
  onNewFile();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
  setWindowTitle("CodeWizard");
  resize(1200, 768);

  // Create all components
  m_menuBar = menuBar();
  m_commandBar = new CommandBar(this);
  m_sidebar = new Sidebar(this);
  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setTabsClosable(true);
  m_tabWidget->setMovable(true);
  m_statusBar = new StatusBar(this);

  // File menu
  QMenu* fileMenu = m_menuBar->addMenu(tr("&File"));

  m_newFileAction = new QAction(tr("&New"), this);
  m_newFileAction->setShortcut(QKeySequence::New);
  connect(m_newFileAction, &QAction::triggered, this, &MainWindow::onNewFile);
  fileMenu->addAction(m_newFileAction);

  m_openFileAction = new QAction(tr("&Open..."), this);
  m_openFileAction->setShortcut(QKeySequence::Open);
  connect(m_openFileAction, &QAction::triggered, this, &MainWindow::onOpenFile);
  fileMenu->addAction(m_openFileAction);

  fileMenu->addSeparator();

  m_saveFileAction = new QAction(tr("&Save"), this);
  m_saveFileAction->setShortcut(QKeySequence::Save);
  m_saveFileAction->setEnabled(false);
  connect(m_saveFileAction, &QAction::triggered, this, &MainWindow::onSaveFile);
  fileMenu->addAction(m_saveFileAction);

  m_saveFileAsAction = new QAction(tr("Save &As..."), this);
  m_saveFileAsAction->setShortcut(QKeySequence::SaveAs);
  connect(m_saveFileAsAction, &QAction::triggered, this, &MainWindow::onSaveFileAs);
  fileMenu->addAction(m_saveFileAsAction);

  fileMenu->addSeparator();

  m_closeTabAction = new QAction(tr("&Close Tab"), this);
  m_closeTabAction->setShortcut(QKeySequence::Close);
  connect(m_closeTabAction, &QAction::triggered, this, &MainWindow::onCloseTab);
  fileMenu->addAction(m_closeTabAction);

  // Connections
  connect(m_commandBar, &CommandBar::commandSubmitted, this, &MainWindow::onCommandSubmitted);
  connect(m_sidebar, &Sidebar::fileOpened, this, &MainWindow::onFileOpenedFromSidebar);
  connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onCloseTab);
  connect(&Theme::ThemeEngine::instance(), &Theme::ThemeEngine::themeChanged, this, &MainWindow::onThemeChanged);
  setupLayout();
  setStatusBar(m_statusBar);
}

void MainWindow::setupLayout()
{
  // Main content area
  // ReSharper disable once CppDFAMemoryLeak
  auto *mainWidget = new QWidget(this);

  // Horizontal splitter: sidebar + tabs
  // ReSharper disable once CppDFAMemoryLeak
  auto *splitter = new QSplitter(Qt::Horizontal, mainWidget);
  splitter->addWidget(m_sidebar);
  splitter->addWidget(m_tabWidget);
  splitter->setStretchFactor(1, 1);// Tabs get more space

  // Vertical layout: command bar + splitter
  // ReSharper disable once CppDFAMemoryLeak
  auto *layout = new QVBoxLayout(mainWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  m_commandBar->setMaximumHeight(48);
  layout->addWidget(m_commandBar);
  layout->addWidget(splitter);

  setCentralWidget(mainWidget);
}

void MainWindow::onNewFile()
{
  auto* newTab = new EditorTab(this);
  newTab->setFilePath(""); // Unsaved file

  connect(newTab, &EditorTab::modificationChanged,
          this, &MainWindow::onModificationChanged);
  connect(newTab, &EditorTab::filePathChanged,
          this, &MainWindow::onFilePathChanged);

  int index = m_tabWidget->addTab(newTab, tr("Untitled"));
  m_tabWidget->setCurrentIndex(index);

  // Enable save actions for new files
  m_saveFileAction->setEnabled(true);
  m_saveFileAsAction->setEnabled(true);
}

void MainWindow::onOpenProject()
{
  const QString path = QFileDialog::getExistingDirectory(this, "Open Project");
  if (!path.isEmpty()) { openProject(path); }
}

void MainWindow::onOpenFile() {
  // Use Qt's file dialog for UI, but Platform API for actual file operations
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open File"), getCurrentWorkingDirectory().native().c_str(),
      tr("All Files (*);;C++ Files (*.cpp *.h *.hpp);;CMake Files (CMakeLists.txt)"));

  if (filePath.isEmpty()) return;

  EditorTab* newTab = new EditorTab(this);

  if (newTab->loadFromFile(filePath)) {
    connect(newTab, &EditorTab::modificationChanged,
            this, &MainWindow::onModificationChanged);
    connect(newTab, &EditorTab::filePathChanged,
            this, &MainWindow::onFilePathChanged);

    Platform::Path path(filePath.toStdString());
    QString fileName = QString::fromStdString(path.filename());
    int index = m_tabWidget->addTab(newTab, fileName);
    m_tabWidget->setCurrentIndex(index);

    m_saveFileAction->setEnabled(true);
    m_saveFileAsAction->setEnabled(true);
  } else {
    delete newTab;
  }
}
void MainWindow::onSaveFile() {
  EditorTab* tab = currentTab();
  if (!tab) return;

  QString filePath = tab->getFilePath();
  if (filePath.isEmpty()) {
    onSaveFileAs();
    return;
  }

  if (tab->saveToFile(filePath)) {
    updateWindowTitle();
  }
}

void MainWindow::onSaveFileAs() {
  EditorTab* tab = currentTab();
  if (!tab) return;

  QString suggestedName = tab->getFilePath().isEmpty() ?
      "untitled.txt" : QString::fromStdString(Platform::Path(tab->getFilePath().toStdString()).filename());

  QString filePath = getSaveFileName(suggestedName);
  if (filePath.isEmpty()) return;

  if (tab->saveToFile(filePath)) {
    Platform::Path path(filePath.toStdString());
    QString fileName = QString::fromStdString(path.filename());
    m_tabWidget->setTabText(m_tabWidget->currentIndex(), fileName);
    updateWindowTitle();
  }
}
void MainWindow::openProject(const QString &projectPath)
{
  m_currentProjectPath = projectPath;
  m_sidebar->projectView()->setRootPath(projectPath);
  setWindowTitle(QString("CodeWizard - %1").arg(QDir(projectPath).dirName()));
}

void MainWindow::onCommandSubmitted(const QString &command)
{
  m_statusBar->showMessage(QString("Command: %1").arg(command), 2000);
}

void MainWindow::onFileOpenedFromSidebar(const QString &filePath)
{
  EditorTab* newTab = new EditorTab(this);

  if (newTab->loadFromFile(filePath)) {
    connect(newTab, &EditorTab::modificationChanged,
            this, &MainWindow::onModificationChanged);
    connect(newTab, &EditorTab::filePathChanged,
            this, &MainWindow::onFilePathChanged);

    Platform::Path path(filePath.toStdString());
    QString fileName = QString::fromStdString(path.filename());
    int index = m_tabWidget->addTab(newTab, fileName);
    m_tabWidget->setCurrentIndex(index);

    m_saveFileAction->setEnabled(true);
    m_saveFileAsAction->setEnabled(true);
  } else {
    delete newTab;
  }
}

void MainWindow::onFilePathChanged(const QString& filepath) {
  EditorTab* senderTab = qobject_cast<EditorTab*>(sender());
  if (!senderTab) return;

  int index = m_tabWidget->indexOf(senderTab);
  if (index == -1) return;

  if (filepath.isEmpty()) {
    m_tabWidget->setTabText(index, tr("Untitled"));
  } else {
    Platform::Path path(filepath.toStdString());
    QString fileName = QString::fromStdString(path.filename());
    m_tabWidget->setTabText(index, fileName);
  }

  updateWindowTitle();
}

void MainWindow::onCloseTab(int index) {
  if (m_tabWidget->count() <= 1) {
    // Don't close last tab, just clear it
    EditorTab* tab = qobject_cast<EditorTab*>(m_tabWidget->widget(index));
    if (tab != nullptr) {
      tab->getEditor()->setDocumentText("");
      tab->setFilePath("");
      tab->setModified(false);
      m_tabWidget->setTabText(index, tr("Untitled"));
    }
    return;
  }

  QWidget* widget = m_tabWidget->widget(index);
  m_tabWidget->removeTab(index);
  delete widget;
}

EditorTab* MainWindow::currentTab() const {
  if (m_tabWidget->count() == 0) return nullptr;
  return qobject_cast<EditorTab*>(m_tabWidget->currentWidget());
}

void MainWindow::onTabChanged(int index) {
  Q_UNUSED(index);
  EditorTab* tab = currentTab();
  bool hasTab = (tab != nullptr);

  m_saveFileAction->setEnabled(hasTab);
  m_saveFileAsAction->setEnabled(hasTab);
  m_closeTabAction->setEnabled(m_tabWidget->count() > 1);

  updateWindowTitle();
}

void MainWindow::updateWindowTitle() {
  EditorTab* tab = currentTab();
  if (!tab) {
    setWindowTitle("CodeWizard");
    return;
  }

  QString fileName = tab->getFilePath().isEmpty() ?
      tr("Untitled") : QString::fromStdString(Platform::Path(tab->getFilePath().toStdString()).filename());
  QString modified = tab->isModified() ? "*" : "";
  QString title = QString("%1%2 - CodeWizard").arg(fileName, modified);
  setWindowTitle(title);
}

void MainWindow::onModificationChanged(bool modified) {
  EditorTab* senderTab = qobject_cast<EditorTab*>(sender());
  if (!senderTab) return;

  int index = m_tabWidget->indexOf(senderTab);
  if (index == -1) return;

  QString tabText = m_tabWidget->tabText(index);
  if (modified && !tabText.endsWith("*")) {
    m_tabWidget->setTabText(index, tabText + "*");
  } else if (!modified && tabText.endsWith("*")) {
    m_tabWidget->setTabText(index, tabText.chopped(1));
  }

  updateWindowTitle();
}

void MainWindow::onDiagnosticsReceived(const QList<LanguageIntelligence::Diagnostic> &diagnostics)
{
  renderDiagnostics(diagnostics);
}

void MainWindow::onThemeChanged()
{
  m_sidebar->applyTheme();
  m_commandBar->applyTheme();
  m_statusBar->applyTheme();
}

void MainWindow::renderDiagnostics(const QList<LanguageIntelligence::Diagnostic> &diagnostics)
{
  int errors = 0, warnings = 0;
  for (const auto &diag : diagnostics) {
    if (diag.severity == LanguageIntelligence::DiagnosticSeverity::Error) {
      errors++;
    } else if (diag.severity == LanguageIntelligence::DiagnosticSeverity::Warning) {
      warnings++;
    }
  }
  m_statusBar->showDiagnostics(errors, warnings);
}

QString MainWindow::getSaveFileName(const QString &suggestedName)
{
  // Use Qt's file dialog for consistent UI across platforms
  return QFileDialog::getSaveFileName(this,
    tr("Save File As"),
    getCurrentWorkingDirectory().native().c_str(),
    tr("All Files (*)"),
    nullptr,
    QFileDialog::DontConfirmOverwrite
  );
}

CodeWizard::Platform::Path MainWindow::getCurrentWorkingDirectory()
{
  // Get current working directory using Platform API
  auto currentPath = Platform::Path::currentWorkingDirectory();
  if (currentPath.exists()) { return currentPath; }
  // Fallback to home directory
  return Platform::Path::homeDirectory();
}
}// namespace CodeWizard::UI
