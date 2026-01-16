#include "EditorImpl.h"
#include "EditorCore.h"
#include "EditorManager.h"
#include "IncrementalParser.h"
#include "SyntaxHighlighter.h"

namespace CodeWizard::Editor::Internal {

EditorImpl::EditorImpl()
    : m_core(std::make_unique<EditorCore>())
    , m_parser(std::make_unique<IncrementalParser>())
    , m_highlighter(std::make_unique<SyntaxHighlighter>())
    , m_manager(std::make_unique<EditorManager>())
{
}

EditorImpl::~EditorImpl() = default;

void EditorImpl::openFile(const std::string& path)
{
    m_manager->openFile(path);
}

void EditorImpl::saveFile(const std::string& path)
{
    m_manager->saveFile(path);
}

std::shared_ptr<ITextBuffer> EditorImpl::buffer() const
{
    return m_manager->buffer();
}

void EditorImpl::attachLanguageServer(const std::string& languageId)
{
    m_manager->attachLanguageServer(languageId);
}

}
