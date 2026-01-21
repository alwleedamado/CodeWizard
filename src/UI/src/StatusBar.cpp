// src/StatusBar.cpp
#include "UI/StatusBar.h"

#include "Theme/ThemeEngine.h"

#include <QHBoxLayout>

namespace CodeWizard::UI {

StatusBar::StatusBar(QWidget* parent) : QStatusBar(parent) {
    applyTheme();
    m_messageLabel = new QLabel(this);
    m_diagnosticsLabel = new QLabel(this);
    m_diagnosticsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    addPermanentWidget(m_messageLabel, 1);
    addPermanentWidget(m_diagnosticsLabel);
}

void StatusBar::showDiagnostics(int errorCount, int warningCount) {
    QString text;
    if (errorCount > 0 || warningCount > 0) {
        text = QString("Errors: %1, Warnings: %2").arg(errorCount).arg(warningCount);
    }
    m_diagnosticsLabel->setText(text);
}

// void StatusBar::showMessage(const QString& message, int timeout) {
//     m_messageLabel->setText(message);
//     if (timeout > 0) {
//         QTimer::singleShot(timeout, this, [this]() {
//             if (m_messageLabel->text() == m_messageLabel->text()) {
//                 m_messageLabel->clear();
//             }
//         });
//     }
// }
void StatusBar::applyTheme() {
  const auto& colors = Theme::ThemeEngine::instance().colors();

  setStyleSheet(QString(
      "QStatusBar {"
      "   background: %1;"
      "   color: %2;"
      "   border-top: 1px solid %3;"
      "}"
      "QLabel[error=\"true\"] { color: %4; font-weight: bold; }"
  ).arg(colors.statusBarBackground.name())
   .arg(colors.statusBarForeground.name())
   .arg(colors.sidebarBorder.name()) // Reuse border color
   .arg(colors.statusBarError.name()));
}
} // namespace CodeWizard::UI
