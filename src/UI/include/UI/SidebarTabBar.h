// include/UI/SidebarTabBar.h
#pragma once

#include <QIcon>
#include <QVector>
#include <QWidget>

class QToolButton;
class QMenu;

namespace CodeWizard::UI {

class SidebarTabBar : public QWidget
{
  Q_OBJECT

public:
  explicit SidebarTabBar(QWidget *parent = nullptr);
  struct TabInfo
  {
    QString title;
    QIcon icon;
    QWidget *widget = nullptr;
  };
  void addTab(const TabInfo &tab);
  void setCurrentIndex(int index);
  [[nodiscard]] int currentIndex() const { return m_currentIndex; }

  [[nodiscard]] int visibleTabCount() const { return m_visibleTabs; }

signals:
  void currentChanged(int index);
  void tabCloseRequested(int index);

protected:
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void onTabClicked(int index);
  void onOverflowMenuTriggered(QAction *action);

private:
  void updateLayout();
  void showOverflowMenu();

  QVector<TabInfo> m_tabs;
  QVector<QToolButton *> m_tabButtons;
  QToolButton *m_overflowButton = nullptr;
  QMenu *m_overflowMenu = nullptr;
  int m_currentIndex = -1;
  int m_visibleTabs = 0;
  int m_maxVisibleTabs = 5;// Adjust based on height
};

}// namespace CodeWizard::UI