#pragma once
#include <QFile>
#include <QHash>
#include <QSharedPointer>
#include <QString>
#include <tree_sitter/api.h>
namespace CodeWizard::Editor {
// Language metadata
struct LanguageInfo
{
  const TSLanguage *language;
  QString name;
  QString queryPath;
  TSQuery *highlightQuery = nullptr;

  LanguageInfo() : language(nullptr), highlightQuery(nullptr) {}
  ~LanguageInfo()
  {
    if (highlightQuery) ts_query_delete(highlightQuery);
  }
};

class TreeSitterManager
{
public:
  static TreeSitterManager &instance();

  // Initialize with path to queries directory
  void initialize(const QString &queriesRootPath);

  // Register a language
  void registerLanguage(const QString &name, const TSLanguage *langFunc(), const QString &querySubdir);

  // Get language info
  std::shared_ptr<LanguageInfo> getLanguage(const QString &name) const;

  // Available languages
  std::ranges::elements_view<std::ranges::ref_view<const std::unordered_map<QString, std::shared_ptr<LanguageInfo>>>, 0>
    availableLanguages() const;

private:
  TreeSitterManager() = default;
  ~TreeSitterManager();

  std::unordered_map<QString, std::shared_ptr<LanguageInfo>> m_languages;
  QString m_queriesRoot;

  bool loadQuery(LanguageInfo &info);
};
}// namespace CodeWizard::Editor