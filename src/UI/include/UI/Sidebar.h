// include/UI/Sidebar.h
#pragma once

#include <QStackedWidget>
#include <QWidget>

namespace CodeWizard::UI {

class ProjectView;
class SidebarTabBar;

class Sidebar : public QWidget
{
  Q_OBJECT

public:
  explicit Sidebar(QWidget *parent = nullptr);

  [[nodiscard]] ProjectView *projectView() const { return m_projectView; }
  void applyTheme();

signals:
  void fileOpened(const QString &filePath);

private slots:
  void onCurrentTabChanged(int index);

private:
  SidebarTabBar *m_tabBar = nullptr;
  QStackedWidget *m_stack = nullptr;
  ProjectView *m_projectView = nullptr;
};

}// namespace CodeWizard::UI