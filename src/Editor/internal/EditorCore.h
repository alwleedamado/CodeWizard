#pragma once

#include <memory>
#include <string>
#include "Editor/ITextBuffer.h"

namespace CodeWizard::Editor::Internal {

/**
 * @brief Core text editing engine.
 *
 * Responsible only for buffer manipulation and file IO.
 * No UI, no language intelligence.
 */
class EditorCore
{
public:
    EditorCore();
    ~EditorCore();

    void openFile(const std::string& path);
    void saveFile(const std::string& path);

    std::shared_ptr<ITextBuffer> buffer() const;

private:
    std::shared_ptr<ITextBuffer> m_buffer;
};

}
