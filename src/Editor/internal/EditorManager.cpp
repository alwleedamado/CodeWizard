#include "EditorManager.h"
#include "EditorCore.h"

namespace CodeWizard::Editor::Internal {

EditorManager::EditorManager()
    : m_core(std::make_unique<EditorCore>())
{
}

EditorManager::~EditorManager() = default;

void EditorManager::openFile(const std::string& path)
{
    m_core->openFile(path);
}

void EditorManager::saveFile(const std::string& path)
{
    m_core->saveFile(path);
}

std::shared_ptr<ITextBuffer> EditorManager::buffer() const
{
    return m_core->buffer();
}

void EditorManager::attachLanguageServer(const std::string& /*languageId*/)
{
    // LanguageIntelligence resolution happens here
}

}
