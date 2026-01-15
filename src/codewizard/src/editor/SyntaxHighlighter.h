#pragma once
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QList>

class SyntaxHighlighter final : public QSyntaxHighlighter
{
public:
  explicit SyntaxHighlighter(QTextDocument *parent = nullptr);
  void setMaxHighlightLength(int maxLength) {m_maxLineLengthForHighlighting = maxLength;}
protected:
  void highlightBlock(const QString &text) override;
private:
  struct HighlightingRule
  {
    QRegularExpression pattern;
    QTextCharFormat format;
  };
  QList<HighlightingRule> highlightingRules;

  QRegularExpression commentStartExpression;
  QRegularExpression commentEndExpression;

  QTextCharFormat keywordFormat;
  QTextCharFormat classFormat;
  QTextCharFormat singleLineCommentFormat;
  QTextCharFormat multiLineCommentFormat;
  QTextCharFormat quotationFormat;
  QTextCharFormat functionFormat;
  int m_maxLineLengthForHighlighting = 100'1000;
};