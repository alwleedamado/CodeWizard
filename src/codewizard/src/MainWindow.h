#pragma once
#include "editor/EditorWidget.h"
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
  void saveFile();
  void quit();
  void updateWindowTitle();
private:
  void setupConnections();
  void saveAsFile();

  Ui::MainWindow *ui;
  EditorWidget* m_editor;
};
