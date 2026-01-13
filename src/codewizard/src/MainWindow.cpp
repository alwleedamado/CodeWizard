//
// Created by cyberbit on 1/13/26.
//

#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setupConnections();
}
MainWindow::~MainWindow() { delete ui; }

void MainWindow::openFile()
{
  QString filePath = QFileDialog::getOpenFileName(
    this, tr("Open File"), "", tr("Text Files (*.txt *.cpp *.h *.hpp *.py *.js);;All Files (*)"));
  if (filePath.isEmpty()) return;
  QFile file(filePath);
  if (!file.open(QFile::ReadOnly)) {
    QMessageBox::warning(this, tr("Error"), tr("Cannot open file:\n%1").arg(file.errorString()));
    return;
  }
  QTextStream textStream(&file);
  ui->codeEditor->setPlainText(textStream.readAll());
  file.close();
  currentFilePath = filePath;
}

void MainWindow::setupConnections()
{
  connect(ui->action_Open, &QAction::triggered, this, &MainWindow::openFile);
  connect(ui->action_Quit, &QAction::triggered, this, &MainWindow::quit);
}
void MainWindow::quit() { close(); }
