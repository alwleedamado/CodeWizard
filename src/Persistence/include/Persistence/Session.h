// include/Persistence/Session.h
#pragma once

#include <Core/Types.h>
#include <Platform/Path.h>
#include <vector>

namespace CodeWizard::Persistence {

struct OpenedFile {
    Platform::Path path;
    size_t cursorLine = 0;
    size_t cursorColumn = 0;
};

class Session {
public:
    void addOpenedFile(const OpenedFile& file);
    void removeOpenedFile(const Platform::Path& path);
    [[nodiscard]] const std::vector<OpenedFile>& openedFiles() const noexcept { return m_openedFiles; }
    void saveToFile(const Platform::Path& sessionFile);
    void loadFromFile(const Platform::Path& sessionFile);

private:
    std::vector<OpenedFile> m_openedFiles;
};

} // namespace CodeWizard::Persistence
