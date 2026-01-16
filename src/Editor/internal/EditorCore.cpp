#include "EditorCore.h"

namespace CodeWizard::Editor::Internal {

EditorCore::EditorCore()
{
}

EditorCore::~EditorCore() = default;

void EditorCore::openFile(const std::string& /*path*/)
{
    // File loading delegated to Platform module later
}

void EditorCore::saveFile(const std::string& /*path*/)
{
    // File saving delegated to Platform module later
}

std::shared_ptr<ITextBuffer> EditorCore::buffer() const
{
    return m_buffer;
}

}
