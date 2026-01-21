// src/UI/Sidebar.cpp
#include "UI/Sidebar.h"
#include "Theme/ThemeEngine.h"
#include "UI/ProjectView.h"
#include "UI/SidebarTabBar.h"

#include <QVBoxLayout>

namespace CodeWizard::UI {

Sidebar::Sidebar(QWidget *parent)
  : QWidget(parent), m_tabBar(new SidebarTabBar(this)), m_stack(new QStackedWidget(this)),
    m_projectView(new ProjectView(this))
{
  applyTheme();
  m_stack->addWidget(m_projectView);

  // Add tab to custom tab bar
  SidebarTabBar::TabInfo projectTab;
  projectTab.title = "Project";
  projectTab.icon = QIcon::fromTheme("folder");
  projectTab.widget = m_projectView;
  m_tabBar->addTab(projectTab);

  // Connect signals
  connect(m_tabBar, &SidebarTabBar::currentChanged, this, &Sidebar::onCurrentTabChanged);
  connect(m_projectView, &ProjectView::fileOpened, this, &Sidebar::fileOpened);

  // Layout
  auto *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_tabBar);
  layout->addWidget(m_stack);

  // Set initial selection
  m_tabBar->setCurrentIndex(0);
}

void Sidebar::onCurrentTabChanged(int index) { m_stack->setCurrentIndex(index); }
void Sidebar::applyTheme()
{
  const auto& colors = Theme::ThemeEngine::instance().colors();

  QString style = QString(
      "QWidget { background: %1; }"
      "QToolButton {"
      "   min-width: 24px; max-width: 24px;"
      "   min-height: 30px;"
      "   background: %2;"
      "   border: 1px solid %3;"
      "   border-right: none;"
      "   padding: 0;"
      "}"
      "QToolButton:hover { background: %4; }"
      "QToolButton:checked { background: %5; }"
  ).arg(colors.sidebarBackground.name())
   .arg(colors.sidebarTabInactive.name())
   .arg(colors.sidebarBorder.name())
   .arg(colors.sidebarTabHover.name())
   .arg(colors.sidebarTabActive.name());

  setStyleSheet(style);
}
}// namespace CodeWizard::UI