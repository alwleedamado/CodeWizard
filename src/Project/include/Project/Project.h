// include/Project/Project.h
#pragma once

#include <Core/Types.h>
#include <Platform/Path.h>
#include <vector>

namespace CodeWizard::Project {

struct File {
    Platform::Path path;
    bool isExcluded = false;
};

class Project {
public:
    explicit Project(const Platform::Path& rootPath);
    [[nodiscard]] const Platform::Path& rootPath() const noexcept { return m_rootPath; }
    [[nodiscard]] const std::vector<File>& files() const noexcept { return m_files; }
    void addFile(const Platform::Path& path);
    void removeFile(const Platform::Path& path);

private:
    void discoverFiles();
    Platform::Path m_rootPath;
    std::vector<File> m_files;
};

} // namespace CodeWizard::Project
