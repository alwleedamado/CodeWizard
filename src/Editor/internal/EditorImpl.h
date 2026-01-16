#pragma once

#include <memory>
#include "Editor/IEditor.h"

namespace CodeWizard::Editor::Internal {

class EditorCore;
class EditorManager;
class IncrementalParser;
class SyntaxHighlighter;

/**
 * @brief Concrete Editor module implementation.
 *
 * Acts as the composition root for all editor subsystems.
 */
class EditorImpl final : public IEditor
{
public:
    EditorImpl();
    ~EditorImpl() override;

    void openFile(const std::string& path) override;
    void saveFile(const std::string& path) override;
    std::shared_ptr<ITextBuffer> buffer() const override;

    void attachLanguageServer(const std::string& languageId) override;

private:
    std::unique_ptr<EditorCore> m_core;
    std::unique_ptr<IncrementalParser> m_parser;
    std::unique_ptr<SyntaxHighlighter> m_highlighter;
    std::unique_ptr<EditorManager> m_manager;
};

}
