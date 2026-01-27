#include "Editor/TreeSitterHighlighter.h"
#include <QApplication>
#include <QDebug>
#include <QPalette>
#include <QString>
#include <QTextBlock>

namespace CodeWizard::Editor {

TreeSitterHighlighter::TreeSitterHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
  m_parser        = ts_parser_new();
  m_debounceTimer = new QTimer(this);
  m_debounceTimer->setSingleShot(true);
  m_debounceTimer->setInterval(50);// 50ms debounce for rapid typing

  connect(m_debounceTimer, &QTimer::timeout, this, &TreeSitterHighlighter::processPendingChanges);

  // Connect to document changes
  connect(document(), &QTextDocument::contentsChange, this, &TreeSitterHighlighter::onDocumentContentsChanged);

  setThemeDefault();
}

TreeSitterHighlighter::~TreeSitterHighlighter()
{
  if (m_tree) ts_tree_delete(m_tree);
  if (m_parser) ts_parser_delete(m_parser);
}

void TreeSitterHighlighter::setThemeDefault()
{
    std::unordered_map<std::string, QTextCharFormat> theme;

    auto add = [&](const char* name, const QColor& color, bool bold = false, bool italic = false) {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        if (bold) fmt.setFontWeight(QFont::Bold);
        if (italic) fmt.setFontItalic(true);
        theme[name] = fmt;
    };

    // Catppuccin Mocha palette
    // https://github.com/catppuccin/catppuccin

    // Variables
    add("variable", QColor("#cdd6f4"));              // Text
    add("variable.member", QColor("#cdd6f4"));       // Text (struct fields)
    add("variable.parameter", QColor("#eba0ac"));    // Maroon
    add("variable.builtin", QColor("#f5e0dc"));      // Rosewater (this, self)

    // Functions
    add("function", QColor("#89b4fa"));              // Blue
    add("function.method", QColor("#89b4fa"));       // Blue
    add("function.call", QColor("#89b4fa"));         // Blue
    add("function.method.call", QColor("#89dceb"));  // Sky (distinguish calls)
    add("function.builtin", QColor("#89dceb"));      // Sky
    add("constructor", QColor("#89b4fa"), true);     // Blue bold

    // Types
    add("type", QColor("#f9e2af"));                  // Yellow
    add("type.builtin", QColor("#f9e2af"));          // Yellow (int, bool)
    add("type.definition", QColor("#f9e2af"), true); // Yellow bold (class names)
    add("property", QColor("#94e2d5"));              // Teal

    // Constants
    add("constant", QColor("#fab387"));              // Peach
    add("constant.builtin", QColor("#fab387"));      // Peach (nullptr)
    add("boolean", QColor("#fab387"));               // Peach

    // Strings
    add("string", QColor("#a6e3a1"));                // Green
    add("string.special", QColor("#f9e2af"));        // Yellow (regex)
    add("string.escape", QColor("#f5c2e7"));         // Pink
    add("string.regex", QColor("#f5c2e7"));          // Pink

    // Keywords
    add("keyword", QColor("#cba6f7"), true);         // Mauve bold
    add("keyword.modifier", QColor("#cba6f7"));      // Mauve (const, static)
    add("keyword.type", QColor("#cba6f7"));          // Mauve (class, struct)
    add("keyword.exception", QColor("#cba6f7"));     // Mauve (try/catch)
    add("keyword.operator", QColor("#89dceb"));      // Sky (new, delete)
    add("keyword.coroutine", QColor("#cba6f7"));     // Mauve (co_await)

    // Modules
    add("module", QColor("#f9e2af"));                // Yellow (namespaces)
    add("namespace", QColor("#f9e2af"));             // Yellow

    // Operators & Punctuation
    add("operator", QColor("#89dceb"));              // Sky (+, -, *, etc.)
    add("punctuation.bracket", QColor("#9399b2"));   // Overlay1
    add("punctuation.delimiter", QColor("#9399b2")); // Overlay1 (; , .)
    add("_parent", QColor("#6c7086"));               // Overlay0 (internal, dim)

    // Comments
    add("comment", QColor("#6c7086"), false, true);  // Overlay0 italic
    add("comment.documentation", QColor("#6c7086"), false, true);

    // Markup (for future use)
    add("markup.heading", QColor("#cba6f7"), true);
    add("markup.link", QColor("#89b4fa"), false, true);

    // Diff/change colors
    add("diff.plus", QColor("#a6e3a1"));             // Green
    add("diff.minus", QColor("#f38ba8"));            // Red
    add("diff.delta", QColor("#89b4fa"));            // Blue

    setTheme(theme);
}

void TreeSitterHighlighter::setTheme(const std::unordered_map<std::string, QTextCharFormat> &theme)
{
  m_theme = theme;
  buildCaptureCache();
  if (m_tree) rehighlight();
}

void TreeSitterHighlighter::buildCaptureCache()
{
  m_captureIndexToFormat.clear();
  if (!m_languageInfo || !m_languageInfo->highlightQuery) return;

  uint32_t count = ts_query_capture_count(m_languageInfo->highlightQuery);
  for (uint32_t i = 0; i < count; ++i) {
    uint32_t len     = 0;
    const char *name = ts_query_capture_name_for_id(m_languageInfo->highlightQuery, i, &len);
    std::string captureName(name, len);

    // Direct match
    auto it = m_theme.find(captureName);
    if (it != m_theme.end()) {
      m_captureIndexToFormat[i] = it->second;
      continue;
    }

    // Try parent categories (e.g., "function.call" -> "function")
    size_t dotPos = captureName.find_last_of('.');
    while (dotPos != std::string::npos) {
      std::string parent = captureName.substr(0, dotPos);
      it                 = m_theme.find(parent);
      if (it != m_theme.end()) {
        m_captureIndexToFormat[i] = it->second;
        break;
      }
      dotPos = captureName.find_last_of('.', dotPos - 1);
    }
  }
}

bool TreeSitterHighlighter::setLanguage(const QString &languageName)
{
  auto langInfo = TreeSitterManager::instance().getLanguage(languageName);
  if (!langInfo || !langInfo->language) {
    emit highlightError(QString("Language not available: %1").arg(languageName));
    return false;
  }

  if (!langInfo->highlightQuery) {
    emit highlightError(QString("No highlight query for: %1").arg(languageName));
    return false;
  }

  clearLanguage();

  m_languageInfo = langInfo;
  if (!ts_parser_set_language(m_parser, m_languageInfo->language)) {
    emit highlightError("Failed to set parser language");
    m_languageInfo.reset();
    return false;
  }
  // Print all available captures in this query
  uint32_t count = ts_query_capture_count(langInfo->highlightQuery);
  qDebug() << "[Highlighter] Available captures for" << languageName << ":";
  for (uint32_t i = 0; i < count; ++i) {
    uint32_t len;
    const char* name = ts_query_capture_name_for_id(langInfo->highlightQuery, i, &len);
    qDebug() << "  " << i << ":" << QString::fromUtf8(name, len);
  }
  buildCaptureCache();
  performFullParse();
  return true;
}

void TreeSitterHighlighter::clearLanguage()
{
  if (m_tree) {
    ts_tree_delete(m_tree);
    m_tree = nullptr;
  }
  m_languageInfo.reset();
  m_lastText.clear();
  m_lastUtf8.clear();
  m_captureIndexToFormat.clear();
}

void TreeSitterHighlighter::forceReparse()
{
  if (m_languageInfo) { performFullParse(); }
}

void TreeSitterHighlighter::onDocumentContentsChanged(int position, int charsRemoved, int charsAdded)
{
  if (!m_languageInfo || !m_tree) return;
  // Update cached text IMMEDIATELY so highlightBlock always has valid data
  m_lastText = document()->toPlainText();
  m_lastUtf8 = m_lastText.toUtf8();

  if (!m_tree) {
    performFullParse();
    return;
  }
  // Debounce rapid changes (typing)
  m_pendingEdit = { position, charsRemoved, charsAdded, true };
  m_debounceTimer->start();
}

void TreeSitterHighlighter::processPendingChanges()
{
  if (!m_pendingEdit.valid || !m_languageInfo) return;

  QElapsedTimer timer;
  timer.start();
  emit parsingStarted();

  performIncrementalParse(m_pendingEdit.position, m_pendingEdit.charsRemoved, m_pendingEdit.charsAdded);

  emit parsingFinished(timer.elapsed(), true);
  m_pendingEdit.valid = false;
}

void TreeSitterHighlighter::performFullParse()
{
  if (!m_languageInfo || m_isParsing) return;

  m_isParsing = true;
  ++m_stats.parseCount;
  ++m_stats.fullParseCount;

  QElapsedTimer timer;
  timer.start();
  emit parsingStarted();

  QString text    = document()->toPlainText();
  QByteArray utf8 = text.toUtf8();

  // Delete old tree
  if (m_tree) {
    ts_tree_delete(m_tree);
    m_tree = nullptr;
  }

  // Parse new content
  TSTree *newTree = ts_parser_parse_string(m_parser,
    nullptr,// No old tree - full parse
    utf8.constData(),
    utf8.size());

  if (newTree) {
    m_tree     = newTree;
    m_lastText = text;
    m_lastUtf8 = utf8;
    m_stats.totalParseTimeMs += timer.elapsed();
    rehighlight();
  } else {
    emit highlightError("Full parse failed");
  }

  m_isParsing = false;
  emit parsingFinished(timer.elapsed(), false);
}
void TreeSitterHighlighter::applyEditToTree(int startByte, int oldEndByte, int newEndByte)
{
  if (!m_tree) return;

  // Get the new document state for calculating new_end_point
  QString newText    = document()->toPlainText();
  QByteArray newUtf8 = newText.toUtf8();

  // Helper to calculate TSPoint (row, column in BYTES)
  auto calcPoint = [](int bytePos, const QByteArray &utf8) -> TSPoint {
    TSPoint point = { 0, 0 };
    if (bytePos <= 0) return point;
    if (bytePos >= utf8.size()) bytePos = utf8.size();

    const char *data = utf8.constData();

    // Count newlines to get row
    int lineStartByte = 0;
    for (int i = 0; i < bytePos; ++i) {
      if (data[i] == '\n') {
        point.row++;
        lineStartByte = i + 1;
      }
    }

    // Column is BYTE offset within the line (not character count)
    point.column = static_cast<uint32_t>(bytePos - lineStartByte);

    return point;
  };

  TSInputEdit edit;
  edit.start_byte   = static_cast<uint32_t>(startByte);
  edit.old_end_byte = static_cast<uint32_t>(oldEndByte);
  edit.new_end_byte = static_cast<uint32_t>(newEndByte);

  // Points in the OLD document
  edit.start_point   = calcPoint(startByte, m_lastUtf8);
  edit.old_end_point = calcPoint(oldEndByte, m_lastUtf8);

  // new_end_point is in the NEW document
  edit.new_end_point = calcPoint(newEndByte, newUtf8);

  qDebug() << "[TreeSitter] Edit:"
           << "bytes:" << startByte << "->" << oldEndByte << "->" << newEndByte << "points:" << edit.start_point.row
           << ":" << edit.start_point.column << "->" << edit.old_end_point.row << ":" << edit.old_end_point.column
           << "->" << edit.new_end_point.row << ":" << edit.new_end_point.column;

  ts_tree_edit(m_tree, &edit);
}

void TreeSitterHighlighter::performIncrementalParse(int position, int charsRemoved, int charsAdded)
{
  if (!m_tree || !m_languageInfo) return;

  ++m_stats.parseCount;
  ++m_stats.incrementalParseCount;

  QString newText    = document()->toPlainText();
  QByteArray newUtf8 = newText.toUtf8();

  // Calculate byte offsets using OLD document state (m_lastText)
  int startByte  = charOffsetToByte(position, m_lastText);
  int oldEndByte = charOffsetToByte(position + charsRemoved, m_lastText);
  int newEndByte = charOffsetToByte(position + charsAdded, newText);

  // CRITICAL: Apply edit to tree BEFORE parsing
  applyEditToTree(startByte, oldEndByte, newEndByte);// <-- CALL IT HERE

  // Now parse with the edited tree as base
  TSInput input = { &newUtf8,
    [](void *payload, uint32_t byte_offset, TSPoint, uint32_t *bytes_read) -> const char * {
      const QByteArray *data = static_cast<const QByteArray *>(payload);
      if (byte_offset >= static_cast<uint32_t>(data->size())) {
        *bytes_read = 0;
        return nullptr;
      }
      *bytes_read = data->size() - byte_offset;
      return data->constData() + byte_offset;
    },
    TSInputEncodingUTF8 };

  TSTree *newTree = ts_parser_parse(m_parser, m_tree, input);

  if (newTree) {
    TSTree *oldTree = m_tree;
    m_tree          = newTree;
    ts_tree_delete(oldTree);

    m_lastText = newText;
    m_lastUtf8 = newUtf8;

    rehighlight();
  } else {
    emit highlightError("Incremental parse failed, falling back to full");
    performFullParse();
  }
}

void TreeSitterHighlighter::highlightBlock(const QString &text)
{
  if (!m_tree || !m_languageInfo || !m_languageInfo->highlightQuery) return;

  ++m_stats.highlightBlockCalls;
  QElapsedTimer timer;
  timer.start();

  QTextBlock block = currentBlock();
  int blockStart   = block.position();
  int blockEnd     = blockStart + text.length();

  // Safety check: ensure our cached text matches current document length
  int docLength = document()->characterCount();
  if (qAbs(m_lastText.length() - docLength) > 1) {// Allow 1 char tolerance for trailing newline
    qDebug() << "[Highlighter] Cache mismatch! Cached:" << m_lastText.length() << "Doc:" << docLength
             << "- forcing reparse";
    // Schedule full reparse
    QMetaObject::invokeMethod(this, [this]() { forceReparse(); }, Qt::QueuedConnection);
    // return;
  }

  // Convert to byte offsets using cached text (must match tree state)
  int startByte = charOffsetToByte(blockStart, m_lastText);
  int endByte   = charOffsetToByte(blockEnd, m_lastText);

  TSQueryCursor *cursor = ts_query_cursor_new();
  ts_query_cursor_set_byte_range(cursor, startByte, endByte);
  ts_query_cursor_exec(cursor, m_languageInfo->highlightQuery, ts_tree_root_node(m_tree));

  TSQueryMatch match;
  int matchCount = 0;
  std::vector<std::tuple<int, int, uint32_t, QString>> debugCaptures;// start, end, index, name

  while (ts_query_cursor_next_match(cursor, &match)) {
    ++matchCount;

    for (uint16_t i = 0; i < match.capture_count; ++i) {
      const TSQueryCapture &capture = match.captures[i];
      uint32_t capStartByte         = ts_node_start_byte(capture.node);
      uint32_t capEndByte           = ts_node_end_byte(capture.node);

      // Convert to char offsets (document-wide)
      int capStart = byteOffsetToChar(capStartByte);
      int capEnd   = byteOffsetToChar(capEndByte);

      // Clamp to block boundaries
      int localStart = qMax(capStart, blockStart) - blockStart;
      int localEnd   = qMin(capEnd, blockEnd) - blockStart;
      int length     = localEnd - localStart;

      if (length > 0 && localStart >= 0 && localEnd <= text.length()) {
        auto it = m_captureIndexToFormat.find(capture.index);
        if (it != m_captureIndexToFormat.end()) {
          setFormat(localStart, length, it->second);

          // Store for debug
          uint32_t nameLen;
          const char *name = ts_query_capture_name_for_id(m_languageInfo->highlightQuery, capture.index, &nameLen);
          debugCaptures.push_back({ localStart, localEnd, capture.index, QString::fromUtf8(name, nameLen) });
        }
      }
    }
  }

  ts_query_cursor_delete(cursor);
  m_stats.totalHighlightTimeMs += timer.elapsed();
  m_stats.lastMatchCount = matchCount;

  // Debug output: show captures for lines containing comments or if few captures
  if (!debugCaptures.empty() && (text.contains("//") || text.contains("/*") || matchCount < 5)) {
    qDebug() << "[Highlight] Block" << block.blockNumber() << ":" << text.left(40);
    for (const auto &[s, e, idx, name] : debugCaptures) {
      qDebug() << "  " << s << "-" << e << ":" << name << "'" << text.mid(s, e - s) << "'";
    }
  }
}

// Offset conversions using cached UTF-8 that corresponds to current tree
int TreeSitterHighlighter::charOffsetToByte(int charPos) const { return charOffsetToByte(charPos, m_lastText); }

int TreeSitterHighlighter::charOffsetToByte(int charPos, const QString &text) const
{
  if (charPos <= 0) return 0;
  if (charPos >= text.size()) return text.toUtf8().size();
  return text.left(charPos).toUtf8().size();
}

int TreeSitterHighlighter::byteOffsetToChar(int bytePos) const
{
  if (bytePos <= 0) return 0;
  if (bytePos >= m_lastUtf8.size()) return m_lastText.size();

  // Decode UTF-8 up to bytePos
  return QString::fromUtf8(m_lastUtf8.left(bytePos)).size();
}

// Debug utilities
void TreeSitterHighlighter::dumpSyntaxTree() const
{
  if (!m_tree) {
    qDebug() << "[TreeSitter] No tree available";
    return;
  }

  std::function<void(TSNode, int)> dump = [&](TSNode node, int indent) {
    QString prefix(indent, ' ');
    if (ts_node_is_null(node)) return;

    const char *type = ts_node_type(node);
    uint32_t start   = ts_node_start_byte(node);
    uint32_t end     = ts_node_end_byte(node);

    qDebug() << prefix << type << "[" << start << "-" << end << "]"
             << QString::fromUtf8(m_lastUtf8.mid(start, end - start)).left(40);

    uint32_t childCount = ts_node_child_count(node);
    for (uint32_t i = 0; i < childCount; ++i) { dump(ts_node_child(node, i), indent + 2); }
  };

  dump(ts_tree_root_node(m_tree), 0);
}

void TreeSitterHighlighter::debugCurrentBlock() const
{
  QTextBlock block = currentBlock();
  qDebug() << "[Debug] Block" << block.blockNumber() << "pos:" << block.position() << "len:" << block.length()
           << "text:" << block.text().left(30);
}

void TreeSitterHighlighter::logNode(const TSNode &node, const char *context) const
{
  if (ts_node_is_null(node)) {
    qDebug() << context << "NULL node";
    return;
  }
  uint32_t start = ts_node_start_byte(node);
  uint32_t end   = ts_node_end_byte(node);
  qDebug() << context << ts_node_type(node) << "bytes:" << start << "-" << end << "chars:" << byteOffsetToChar(start)
           << "-" << byteOffsetToChar(end);
}

std::string TreeSitterHighlighter::nodeString(const TSNode &node) const
{
  if (ts_node_is_null(node)) return "NULL";
  uint32_t start = ts_node_start_byte(node);
  uint32_t end   = ts_node_end_byte(node);
  return m_lastUtf8.mid(start, end - start).toStdString();
}

}// namespace CodeWizard::Editor