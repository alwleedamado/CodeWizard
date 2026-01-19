// include/UI/MainWindow.h
#pragma once

#include <QMainWindow>

namespace CodeWizard::UI {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
};

} // namespace CodeWizard::UI
