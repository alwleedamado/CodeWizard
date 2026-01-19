// src/MainWindow.cpp
#include "UI/MainWindow.h"

namespace CodeWizard::UI {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("CodeWizard");
}

MainWindow::~MainWindow() = default;

} // namespace CodeWizard::UI
