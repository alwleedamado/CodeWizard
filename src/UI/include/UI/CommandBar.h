// include/UI/CommandBar.h
#pragma once

#include <QWidget>
#include <QLineEdit>

namespace CodeWizard::UI {

class CommandBar : public QWidget {
    Q_OBJECT

public:
    explicit CommandBar(QWidget* parent = nullptr);
  void applyTheme();

  [[nodiscard]] QLineEdit* inputField() { return m_input; }

signals:
    void commandSubmitted(const QString& command);

private:
    QLineEdit* m_input = nullptr;
};

} // namespace CodeWizard::UI
