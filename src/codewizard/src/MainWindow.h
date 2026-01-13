#pragma once
#include "ui/ui_MainWindow.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
private slots:
  void openFile();
  void setupConnections();
  void quit();
private:
  Ui::MainWindow *ui;
  QString currentFilePath;
};
