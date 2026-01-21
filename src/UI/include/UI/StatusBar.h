// include/UI/StatusBar.h
#pragma once

#include <QStatusBar>
#include <QLabel>

namespace CodeWizard::UI {

class StatusBar : public QStatusBar {
    Q_OBJECT

public:
    explicit StatusBar(QWidget* parent = nullptr);

    void showDiagnostics(int errorCount, int warningCount);
    void applyTheme();

  private:
    QLabel* m_messageLabel = nullptr;
    QLabel* m_diagnosticsLabel = nullptr;
};

} // namespace CodeWizard::UI
