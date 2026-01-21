// src/UI/ProjectView.cpp
#include "UI/ProjectView.h"
#include <QFileIconProvider>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QScrollArea>
#include <QTreeView>
#include <QVBoxLayout>

namespace CodeWizard::UI {

ProjectView::ProjectView(QWidget* parent) : QWidget(parent), m_headerWidget(new QWidget(this)), m_treeView(new QTreeView(this)) {
    // Header widget with placeholder

    auto* headerLayout = new QHBoxLayout(m_headerWidget);
    headerLayout->setContentsMargins(8, 4, 8, 4);
    headerLayout->addStretch(); // Placeholder for future buttons

    // File system model
    m_model = new QFileSystemModel(this);
    m_model->setRootPath("");
    m_model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    m_model->setIconProvider(new QFileIconProvider());

    // Tree view
    m_treeView->setModel(m_model);
    m_treeView->setHeaderHidden(true);
    m_treeView->setIndentation(16);
    m_treeView->setAnimated(false);
    m_treeView->setSortingEnabled(true);
    m_treeView->sortByColumn(0, Qt::AscendingOrder);

    // Hide size/date columns
    for (int i = 1; i < m_model->columnCount(); ++i) {
        m_treeView->hideColumn(i);
    }

    // Scroll area for both directions
    auto* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(m_treeView);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Main layout
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_headerWidget);
    layout->addWidget(scrollArea);

    connect(m_treeView, &QTreeView::doubleClicked,
            this, &ProjectView::onFileDoubleClicked);
}

void ProjectView::setRootPath(const QString& path) {
    m_rootPath = path;
    m_treeView->setRootIndex(m_model->index(path));
}

void ProjectView::onFileDoubleClicked(const QModelIndex& index) {
    if (m_model->isDir(index)) return;
    QString const path = m_model->filePath(index);
    emit fileOpened(path);
}

} // namespace CodeWizard::UI