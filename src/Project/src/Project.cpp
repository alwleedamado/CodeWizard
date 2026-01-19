// src/Project.cpp
#include "Project/Project.h"
#include <Platform/FileSystem.h>

namespace CodeWizard::Project {

Project::Project(const Platform::Path& rootPath) : m_rootPath(rootPath) {
    discoverFiles();
}

void Project::addFile(const Platform::Path& path) {
    m_files.push_back({path});
}

void Project::removeFile(const Platform::Path& path) {
    m_files.erase(std::remove_if(m_files.begin(), m_files.end(),
        [&](const File& f) { return f.path == path; }), m_files.end());
}

void Project::discoverFiles() {
    auto result = Platform::listDirectory(m_rootPath);
    if (result.hasValue()) {
        for (const auto& info : result.value()) {
            if (info.isFile) {
                m_files.push_back({info.path});
            }
        }
    }
}

} // namespace CodeWizard::Project
