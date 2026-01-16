#pragma once
#include <string>

namespace CodeWizard::Editor {

class ITextBuffer
{
public:
    virtual ~ITextBuffer() = default;

    virtual const std::string& text() const = 0;
    virtual void setText(const std::string& newText) = 0;

    virtual void insert(size_t pos, const std::string& str) = 0;
    virtual void erase(size_t pos, size_t len) = 0;
};

} // namespace CodeWizard::Editor
