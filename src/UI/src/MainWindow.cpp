#include "UI/MainWindow.h"

#include "Theme/ThemeEngine.h"
#include "UI/CommandBar.h"
#include "UI/ProjectView.h"
#include "UI/Sidebar.h"
#include "UI/StatusBar.h"
#include <Editor/Document.h>
#include <Platform/Path.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QVBoxLayout>

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

  // Menu
  auto *fileMenu = m_menuBar->addMenu("&File");
  fileMenu->addAction("&New", this, &MainWindow::onNewFile);
  fileMenu->addAction("&Open File...", this, &MainWindow::onOpenFile);
  fileMenu->addAction("Open &Project...", this, &MainWindow::onOpenProject);
  fileMenu->addSeparator();
  fileMenu->addAction("&Quit", qApp, &QApplication::quit);

  // Connections
  connect(m_commandBar, &CommandBar::commandSubmitted, this, &MainWindow::onCommandSubmitted);
  connect(m_sidebar, &Sidebar::fileOpened, this, &MainWindow::onFileOpenedFromSidebar);
  connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
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
  auto *doc = new Editor::Document;
  auto *tab = createTab(doc);
  m_tabWidget->setCurrentWidget(tab);
}

void MainWindow::onOpenFile()
{
  QString path = QFileDialog::getOpenFileName(this, "Open File");
  if (!path.isEmpty()) { openFile(path); }
}

void MainWindow::onOpenProject()
{
  QString path = QFileDialog::getExistingDirectory(this, "Open Project");
  if (!path.isEmpty()) { openProject(path); }
}

void MainWindow::openFile(const QString &filePath)
{
  auto *doc = new Editor::Document(Platform::Path(filePath.toStdString()));
  if (!doc->loadFromFile().hasValue()) {
    QMessageBox::warning(this, "Error", "Failed to load file");
    delete doc;
    return;
  }
  auto *tab = createTab(doc);
  m_tabWidget->setCurrentWidget(tab);
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

void MainWindow::onFileOpenedFromSidebar(const QString &filePath) { openFile(filePath); }

EditorTab *MainWindow::createTab(Editor::Document *doc)
{
  auto *tab = new EditorTab(doc, this);
  QString title = doc->isUntitled() ? "Untitled" : QString::fromStdString(doc->filePath().filename());
  m_tabWidget->addTab(tab, title);

  connect(tab, &EditorTab::modificationChanged, this, &MainWindow::onModificationChanged);
  connect(tab, &EditorTab::diagnosticsReceived, this, &MainWindow::onDiagnosticsReceived);

  m_docToTab[doc] = tab;
  m_currentDocument = doc;
  return tab;
}
void MainWindow::updateTabTitle(EditorTab *tab) {}

void MainWindow::onTabCloseRequested(int index)
{
  auto *widget = m_tabWidget->widget(index);
  auto it = m_docToTab.key(static_cast<EditorTab *>(widget));
  if (it) {
    m_docToTab.remove(it);
    delete it;
  }
  m_tabWidget->removeTab(index);
}

void MainWindow::onModificationChanged(bool modified)
{
  auto *tab = qobject_cast<EditorTab *>(sender());
  if (!tab) return;

  auto *it = m_docToTab.key(tab);
  if (!it) return;

  QString const title = it->isUntitled() ? "Untitled[*]" : QString::fromStdString(it->filePath().filename() + "[*]");
  m_tabWidget->setTabText(m_tabWidget->indexOf(tab), title);
  m_tabWidget->setWindowModified(modified);
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

}// namespace CodeWizard::UI
