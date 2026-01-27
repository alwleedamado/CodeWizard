#include "Editor/TreeSitterManager.h"
#include <QDebug>
#include <QDir>

// External language functions (declare these based on your linked libraries)
extern "C" {
const TSLanguage *tree_sitter_c();
const TSLanguage *tree_sitter_cpp();
const TSLanguage *tree_sitter_go();
const TSLanguage *tree_sitter_javascript();
const TSLanguage *tree_sitter_typescript();
const TSLanguage *tree_sitter_rust();
}
namespace CodeWizard::Editor {
TreeSitterManager::~TreeSitterManager()
{
  // LanguageInfo destructors clean up queries
}

TreeSitterManager &TreeSitterManager::instance()
{
  static TreeSitterManager instance;
  return instance;
}

void TreeSitterManager::initialize(const QString &queriesRootPath)
{
  m_queriesRoot = queriesRootPath;

  // Register built-in languages
  registerLanguage("c", tree_sitter_c, "c");
  registerLanguage("cpp", tree_sitter_cpp, "cpp");
  registerLanguage("go", tree_sitter_go, "go");
  registerLanguage("javascript", tree_sitter_javascript, "javascript");
  registerLanguage("typescript", tree_sitter_typescript, "typescript");
  registerLanguage("rust", tree_sitter_rust, "rust");
}

void TreeSitterManager::registerLanguage(const QString &name, const TSLanguage *langFunc(), const QString &querySubdir)
{
  auto info       = std::make_shared<LanguageInfo>();
  info->name      = name;
  info->language  = langFunc();
  info->queryPath = m_queriesRoot + "/" + querySubdir + "/highlights.scm";

  if (loadQuery(*info)) {
    m_languages[name] = info;
    qDebug() << "Registered language:" << name;
  } else {
    qWarning() << "Failed to load queries for:" << name;
  }
}


bool TreeSitterManager::loadQuery(LanguageInfo &info)
{

  QByteArray combinedQuery;

  // Handle inheritance manually
  if (info.name == "cpp") {
    // Load C first (parent)
    QFile cFile(m_queriesRoot + "/c/highlights.scm");
    if (cFile.open(QIODevice::ReadOnly)) {
      combinedQuery += cFile.readAll();
      combinedQuery += "\n"; // Ensure newline separation
      qDebug() << "Loaded inherited queries from c";
    }
  }
  // Add other inheritance rules as needed:
  // else if (querySubdir == "typescript") { load javascript first }
  // else if (querySubdir == "tsx") { load typescript, then javascript }

  // Load the language's own queries
  QFile file(info.queryPath);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "Cannot open query file:" << info.queryPath;
    return false;
  }

  combinedQuery += file.readAll();

  // Create query from combined source
  uint32_t errorOffset;
  TSQueryError errorType;
  info.highlightQuery = ts_query_new(
      info.language,
      combinedQuery.constData(),
      combinedQuery.size(),
      &errorOffset,
      &errorType
  );

  if (!info.highlightQuery) {
    qWarning() << "Query error at offset" << errorOffset
               << "in combined query for" << info.queryPath;
    return false;
  }

  return true;
}

std::shared_ptr<LanguageInfo> TreeSitterManager::getLanguage(const QString &name) const
{
  return m_languages.at(name);
}

std::ranges::elements_view<std::ranges::ref_view<const std::unordered_map<QString, std::shared_ptr<LanguageInfo>>>, 0>
  TreeSitterManager::availableLanguages() const { return std::views::keys(m_languages); }
}// namespace CodeWizard::Editor