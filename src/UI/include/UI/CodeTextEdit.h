#pragma once

#include <QPlainTextEdit>

namespace CodeWizard::UI {

class CodeTextEdit final : public QPlainTextEdit
{
  Q_OBJECT

public:
  explicit CodeTextEdit(QWidget *parent = nullptr);

signals:
  void userInput(const QString &text, int position);
  void backspacePressed(int position);
  void deletePressed(int position);
  void enterPressed(int position);
  void cutRequested(int start, int end);
  void pasteRequested(int position);

protected:
  void keyPressEvent(QKeyEvent *event) override;

private:
  [[nodiscard]] int getCurrentPosition() const;
};

}// namespace CodeWizard::UI