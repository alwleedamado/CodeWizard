//
// Created by cyberbit on 1/13/26.
//

#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), m_editor(new EditorWidget(this))
{
  ui->setupUi(this);
  setCentralWidget(m_editor);
  setupConnections();
}
MainWindow::~MainWindow() { delete ui; }

void MainWindow::openFile()
{
  if (const QString fileName = QFileDialog::getOpenFileName(this); !fileName.isEmpty()) {
    m_editor->loadFromFile(fileName);
  }
  updateWindowTitle();
}
void MainWindow::saveFile()
{
  if (m_editor->filePath().isEmpty()) {
    saveAsFile();
  } else {
    m_editor->saveToFile(m_editor->filePath());
  }
  updateWindowTitle();
}

void MainWindow::setupConnections()
{
  connect(ui->action_Open, &QAction::triggered, this, &MainWindow::openFile);
  connect(ui->action_Quit, &QAction::triggered, this, &MainWindow::quit);
  connect(ui->action_Save, &QAction::triggered, this, &MainWindow::saveFile);
  connect(m_editor, &EditorWidget::modificationChanged, this, &MainWindow::updateWindowTitle);
}
void MainWindow::saveAsFile()
{
  if (const QString fileName = QFileDialog::getSaveFileName(this); !fileName.isEmpty()) {
    m_editor->saveToFile(fileName);
  }
}
void MainWindow::quit() { close(); }
void MainWindow::updateWindowTitle()
{
  QString filename;
  if (m_editor == nullptr) return;
  if (m_editor->filePath().isEmpty()) {
    filename = "Untitled";
  } else {
    filename = QFileInfo(m_editor->filePath()).fileName();
  }
  QString title = tr("%1[*] - %2").arg(filename, "CodeWizard");
  setWindowTitle(title);
  setWindowModified(m_editor->isModified());
}
