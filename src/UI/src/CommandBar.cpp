// src/CommandBar.cpp
#include "UI/CommandBar.h"

#include "Theme/ThemeEngine.h"


#include <QHBoxLayout>
#include <QLabel>

namespace CodeWizard::UI {

CommandBar::CommandBar(QWidget* parent) : QWidget(parent) {
    setStyleSheet("background: #f0f0f0; border-bottom: 1px solid #ccc;");

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 4, 8, 4);

    // auto* label = new QLabel(">");
    // label->setStyleSheet("font-weight: bold; color: #666;");
    // m_input = new QLineEdit(this);
    // m_input->setPlaceholderText("Type a command or search...");
    //
    // layout->addWidget(label);
    // layout->addWidget(m_input);
    // layout->setStretch(1, 1);
    //
    // connect(m_input, &QLineEdit::returnPressed, this, [this]() {
    //     emit commandSubmitted(m_input->text());
    //     m_input->clear();
    // });
}
void CommandBar::applyTheme() {
  const auto& colors = Theme::ThemeEngine::instance().colors();

  setStyleSheet(QString(
      "QWidget {"
      "   background: %1;"
      "   border-bottom: 1px solid %2;"
      "}"
      "QLineEdit {"
      "   background: %3;"
      "   color: %4;"
      "   border: 1px solid %2;"
      "   border-radius: 3px;"
      "   padding: 4px 8px;"
      "}"
  ).arg(colors.sidebarBackground.name())
   .arg(colors.sidebarBorder.name())
   .arg(colors.editorBackground.name())
   .arg(colors.editorForeground.name()));
}
} // namespace CodeWizard::UI
