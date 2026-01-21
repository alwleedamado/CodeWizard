// src/UI/SidebarTabBar.cpp
#include "UI/SidebarTabBar.h"
#include <QToolButton>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QStyle>

namespace CodeWizard::UI {

SidebarTabBar::SidebarTabBar(QWidget* parent) : QWidget(parent) {
    setFixedWidth(24); // Fixed 24px width

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addStretch();
    // Overflow button (hidden by default)
    m_overflowButton = new QToolButton(this);
    m_overflowButton->setFixedSize(24, 24);
    m_overflowButton->setIcon(QIcon::fromTheme("document-open"));
    m_overflowButton->setPopupMode(QToolButton::InstantPopup);
    m_overflowButton->hide();
    layout->addWidget(m_overflowButton);

    connect(m_overflowButton, &QToolButton::clicked,
            this, &SidebarTabBar::showOverflowMenu);
}

void SidebarTabBar::addTab(const TabInfo& tab) {
    m_tabs.append(tab);
    updateLayout();
}

void SidebarTabBar::setCurrentIndex(int index) {
    if (index < 0 || index >= m_tabs.size()) return;

    m_currentIndex = index;
    emit currentChanged(index);

    // Update button states
    for (int i = 0; i < m_tabButtons.size(); ++i) {
        m_tabButtons[i]->setChecked(i == index);
    }
}

void SidebarTabBar::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateLayout();
}

void SidebarTabBar::updateLayout() {
    // Clear existing buttons
    for (auto* button : m_tabButtons) {
        delete button;
    }
    m_tabButtons.clear();

    // Calculate how many tabs fit
    int availableHeight = height() - m_overflowButton->height();
    m_visibleTabs = qMin(m_tabs.size(), availableHeight / 30); // 30px per tab

    // Create visible tab buttons
    auto* layout = dynamic_cast<QVBoxLayout*>(this->layout());
    for (int i = 0; i < m_visibleTabs; ++i) {
        auto* button = new QToolButton(this);
        button->setFixedSize(24, 30);
        button->setIcon(m_tabs[i].icon);
        button->setCheckable(true);
        button->setChecked(i == m_currentIndex);
        button->setToolTip(m_tabs[i].title);

        // Store index in property
        button->setProperty("tabIndex", i);
        connect(button, &QToolButton::clicked, this, [this, i]() {
            onTabClicked(i);
        });

        m_tabButtons.append(button);
        layout->insertWidget(i, button);
    }

    // Handle overflow
    if (m_visibleTabs < m_tabs.size()) {
        m_overflowButton->show();
        // Rebuild overflow menu
        delete m_overflowMenu;
        m_overflowMenu = new QMenu(this);
        for (int i = m_visibleTabs; i < m_tabs.size(); ++i) {
            QAction* action = m_overflowMenu->addAction(m_tabs[i].icon, m_tabs[i].title);
            action->setData(i);
        }
        m_overflowButton->setMenu(m_overflowMenu);
    } else {
        m_overflowButton->hide();
    }
}

void SidebarTabBar::onTabClicked(int index) {
    setCurrentIndex(index);
}

void SidebarTabBar::showOverflowMenu() {
    if (m_overflowMenu) {
        m_overflowMenu->popup(m_overflowButton->mapToGlobal(
            QPoint(0, m_overflowButton->height())));
    }
}

void SidebarTabBar::onOverflowMenuTriggered(QAction* action) {
    int index = action->data().toInt();
    setCurrentIndex(index);
}

} // namespace CodeWizard::UI