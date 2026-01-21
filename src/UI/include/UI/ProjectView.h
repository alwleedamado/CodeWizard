// include/UI/ProjectView.h
#pragma once

#include <QWidget>

class QTreeView;
class QFileSystemModel;

namespace CodeWizard::UI {

class ProjectView : public QWidget {
  Q_OBJECT

public:
  explicit ProjectView(QWidget* parent = nullptr);

  void setRootPath(const QString& path);
  [[nodiscard]] const QString& rootPath() const { return m_rootPath; }

  signals:
      void fileOpened(const QString& filePath);

private slots:
    void onFileDoubleClicked(const QModelIndex& index);

private:
  QWidget* m_headerWidget = nullptr;
  QTreeView* m_treeView = nullptr;
  QFileSystemModel* m_model = nullptr;
  QString m_rootPath;
};

} // namespace CodeWizard::UI