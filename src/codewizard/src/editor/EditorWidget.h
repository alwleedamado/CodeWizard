#pragma once
#include <QPlainTextEdit>


class EditorWidget final : public QWidget
{
  Q_OBJECT
public:
  explicit EditorWidget(QWidget* parent = nullptr);
  ~EditorWidget() override;

  bool loadFromFile(const QString& filepath);
  bool saveToFile(const QString& filepath);

  [[nodiscard]] bool isModified() const { return m_widget->document()->isModified(); }
  // ReSharper disable once CppMemberFunctionMayBeConst
  void setModified(bool modified) { m_widget->document()->setModified(modified); }
  [[nodiscard]] QString filePath() const { return m_filePath; }
  void setFilePath(const QString& filepath) { m_filePath = filepath; }

  [[nodiscard]] const QPlainTextEdit* get_widget() const { return m_widget; }
  QPlainTextEdit* get_widget() { return m_widget; }

private:
  QPlainTextEdit* m_widget;
  QString m_filePath;

signals:
  void modificationChanged(bool modified);
  void filePathChanged(const QString& filepath);

private slots:
  void onModificationChanged();
};