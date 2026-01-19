// src/Session.cpp
#include "Persistence/Session.h"
#include <Platform/FileSystem.h>
#include <nlohmann/json.hpp>

namespace CodeWizard::Persistence {

void Session::addOpenedFile(const OpenedFile& file) {
    m_openedFiles.push_back(file);
}

void Session::removeOpenedFile(const Platform::Path& path) {
    m_openedFiles.erase(std::remove_if(m_openedFiles.begin(), m_openedFiles.end(),
        [&](const OpenedFile& f) { return f.path == path; }), m_openedFiles.end());
}

void Session::saveToFile(const Platform::Path& sessionFile) {
    nlohmann::json j;
    for (const auto& file : m_openedFiles) {
        nlohmann::json f;
        f["path"] = file.path.native();
        f["line"] = file.cursorLine;
        f["column"] = file.cursorColumn;
        j["openedFiles"].push_back(f);
    }
    Platform::writeFile(sessionFile, j.dump(2));
}

void Session::loadFromFile(const Platform::Path& sessionFile) {
    auto result = Platform::readFile(sessionFile);
    if (!result.hasValue()) return;

    try {
        auto j = nlohmann::json::parse(result.value());
        m_openedFiles.clear();
        for (const auto& f : j["openedFiles"]) {
            OpenedFile file;
            file.path = Platform::Path(f["path"].get<std::string>());
            file.cursorLine = f["line"].get<size_t>();
            file.cursorColumn = f["column"].get<size_t>();
            m_openedFiles.push_back(file);
        }
    } catch (...) {
        // Ignore parse errors
    }
}

} // namespace CodeWizard::Persistence
