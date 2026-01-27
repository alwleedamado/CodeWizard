#pragma once
#include "Editor/TreeSitterManager.h"
#include <QElapsedTimer>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTimer>
#include <string>
#include <unordered_map>
#include <vector>

namespace CodeWizard::Editor {

struct HighlightStatistics {
    int parseCount = 0;
    int incrementalParseCount = 0;
    int fullParseCount = 0;
    int highlightBlockCalls = 0;
    qint64 totalParseTimeMs = 0;
    qint64 totalHighlightTimeMs = 0;
    int lastMatchCount = 0;

    void reset() { *this = HighlightStatistics{}; }
};

class TreeSitterHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit TreeSitterHighlighter(QTextDocument* parent);
    ~TreeSitterHighlighter() override;

    bool setLanguage(const QString& languageName);
    void clearLanguage();

    void setTheme(const std::unordered_map<std::string, QTextCharFormat>& theme);
    void setThemeDefault();

    // Debug interface
    HighlightStatistics statistics() const { return m_stats; }
    void resetStatistics() { m_stats.reset(); }

    // Dump current tree structure to qDebug
    void dumpSyntaxTree() const;

    // Dump query captures for current block (call from highlightBlock for debugging)
    void debugCurrentBlock() const;

    // Force full reparse (useful for recovery)
    void forceReparse();

    // Check if parser is healthy
    bool isHealthy() const { return m_parser && m_tree && m_languageInfo; }

signals:
    void parsingStarted();
    void parsingFinished(qint64 durationMs, bool incremental);
    void highlightError(const QString& error);

protected:
    void highlightBlock(const QString& text) override;

private slots:
    void onDocumentContentsChanged(int position, int charsRemoved, int charsAdded);
    void processPendingChanges();

private:
    // Tree-sitter core
    TSParser* m_parser = nullptr;
    TSTree* m_tree = nullptr;
    std::shared_ptr<LanguageInfo> m_languageInfo;

    // Document state tracking (for incremental sync)
    QString m_lastText;        // Previous document text (UTF-16)
    QByteArray m_lastUtf8;     // Previous document text (UTF-8)

    // Coalescing rapid edits
    QTimer* m_debounceTimer;
    struct PendingEdit {
        int position;
        int charsRemoved;
        int charsAdded;
        bool valid = false;
    } m_pendingEdit;

    // Theme
    std::unordered_map<std::string, QTextCharFormat> m_theme;
    std::unordered_map<uint32_t, QTextCharFormat> m_captureIndexToFormat; // Cached mapping

    // Debug/Stats
    mutable HighlightStatistics m_stats;
    bool m_isParsing = false;

    // Offset conversion (uses m_lastUtf8 which corresponds to current tree)
    int charOffsetToByte(int charPos) const;
    int byteOffsetToChar(int bytePos) const;
    int charOffsetToByte(int charPos, const QString& text) const;

    // Parsing
    void performIncrementalParse(int position, int charsRemoved, int charsAdded);
    void performFullParse();
    void applyEditToTree(int startByte, int oldEndByte, int newEndByte);

    // Highlighting helpers
    void buildCaptureCache();
    void highlightBlockRange(TSQueryCursor* cursor, const QString& text, int blockStart, int blockEnd);

    // Debug helpers
    void logNode(const TSNode& node, const char* context = "") const;
    std::string nodeString(const TSNode& node) const;
};

} // namespace CodeWizard::Editor