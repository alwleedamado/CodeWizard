#pragma once
#include <string>
#include <memory>

namespace CodeWizard::Editor {

class ITextBuffer;

class IEditor
{
public:
    virtual ~IEditor() = default;

    virtual void openFile(const std::string& path) = 0;
    virtual void saveFile(const std::string& path) = 0;
    virtual std::shared_ptr<ITextBuffer> buffer() const = 0;

    // Connect to Language Intelligence
    virtual void attachLanguageServer(const std::string& languageId) = 0;
};

} // namespace CodeWizard::Editor
