#pragma once

#include <memory>
#include <string>
#include "Editor/IEditor.h"
#include "LanguageIntelligence/ILanguageIntelligence.h"

namespace CodeWizard::Editor::Internal {

/**
 * @brief High-level editor orchestrator.
 *
 * Bridges EditorCore with LanguageIntelligence.
 */
class EditorManager : public IEditor
{
public:
    EditorManager();
    ~EditorManager() override;

    void openFile(const std::string& path) override;
    void saveFile(const std::string& path) override;

    std::shared_ptr<ITextBuffer> buffer() const override;

    void attachLanguageServer(const std::string& languageId) override;

private:
    std::unique_ptr<class EditorCore> m_core;
    std::shared_ptr<LanguageIntelligence::ILanguageIntelligence> m_li;
};

}
