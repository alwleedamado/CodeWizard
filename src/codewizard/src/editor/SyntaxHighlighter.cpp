
#include "SyntaxHighlighter.h"
SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
  HighlightingRule rule;

  keywordFormat.setForeground(QColor(43, 92, 138));
  keywordFormat.setFontWeight(QFont::Bold);
  const QString keywordPatterns[] = { QStringLiteral("\\bchar\\b"),
    QStringLiteral("\\bclass\\b"),
    QStringLiteral("\\bconst\\b"),
    QStringLiteral("\\bdouble\\b"),
    QStringLiteral("\\benum\\b"),
    QStringLiteral("\\bexplicit\\b"),
    QStringLiteral("\\bfriend\\b"),
    QStringLiteral("\\binline\\b"),
    QStringLiteral("\\bint\\b"),
    QStringLiteral("\\blong\\b"),
    QStringLiteral("\\bnamespace\\b"),
    QStringLiteral("\\boperator\\b"),
    QStringLiteral("\\bprivate\\b"),
    QStringLiteral("\\bprotected\\b"),
    QStringLiteral("\\bpublic\\b"),
    QStringLiteral("\\bshort\\b"),
    QStringLiteral("\\bsignals\\b"),
    QStringLiteral("\\bsigned\\b"),
    QStringLiteral("\\bslots\\b"),
    QStringLiteral("\\bstatic\\b"),
    QStringLiteral("\\bstruct\\b"),
    QStringLiteral("\\btemplate\\b"),
    QStringLiteral("\\btypedef\\b"),
    QStringLiteral("\\btypename\\b"),
    QStringLiteral("\\bunion\\b"),
    QStringLiteral("\\bunsigned\\b"),
    QStringLiteral("\\bvirtual\\b"),
    QStringLiteral("\\bvoid\\b"),
    QStringLiteral("\\bvolatile\\b"),
    QStringLiteral("\\bbool\\b") };
  for (const QString &pattern : keywordPatterns) {
    rule.pattern = QRegularExpression(pattern);
    rule.format = keywordFormat;
    highlightingRules.append(rule);
  }
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
  if (text.size() > m_maxLineLengthForHighlighting)
    return;
  for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
    QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch const match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }
}
