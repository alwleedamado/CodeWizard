// src/Path.cpp
#include "Platform/Path.h"
#include <Core/Logger.h>
#include <filesystem>

namespace fs = std::filesystem;
using namespace CodeWizard::Core;

namespace CodeWizard::Platform {

Path::Path(std::string_view path) : m_path(path) {
    normalize();
}

void Path::normalize() {
    if (m_path.empty()) return;
    try {
        m_path = fs::path(m_path).lexically_normal().string();
    } catch (const std::exception& e) {
        CW_LOG_WARN("Failed to normalize path: " + std::string(e.what()));
    }
}

bool Path::isAbsolute() const noexcept {
    try {
        return fs::path(m_path).is_absolute();
    } catch (...) {
        return false;
    }
}

bool Path::isRelative() const noexcept {
    return !isAbsolute();
}

bool Path::exists() const noexcept {
    try {
        return fs::exists(fs::path(m_path));
    } catch (...) {
        return false;
    }
}

bool Path::isDirectory() const noexcept {
    try {
        return fs::is_directory(fs::path(m_path));
    } catch (...) {
        return false;
    }
}

bool Path::isFile() const noexcept {
    try {
        return fs::is_regular_file(fs::path(m_path));
    } catch (...) {
        return false;
    }
}

Path Path::parent() const {
    try {
        return Path(fs::path(m_path).parent_path().string());
    } catch (...) {
        return Path{};
    }
}

std::string Path::filename() const {
    try {
        return fs::path(m_path).filename().string();
    } catch (...) {
        return {};
    }
}

std::string Path::extension() const {
    try {
        return fs::path(m_path).extension().string();
    } catch (...) {
        return {};
    }
}

Path Path::absolute() const {
    try {
        return Path(fs::absolute(fs::path(m_path)).string());
    } catch (...) {
        return *this;
    }
}

Path Path::operator/(const Path& other) const {
    try {
        return Path((fs::path(m_path) / fs::path(other.m_path)).string());
    } catch (...) {
        return Path{};
    }
}

Path Path::operator/(std::string_view other) const {
    try {
        return Path((fs::path(m_path) / fs::path(other)).string());
    } catch (...) {
        return Path{};
    }
}

Path Path::currentWorkingDirectory() {
    try {
        return Path(fs::current_path().string());
    } catch (...) {
        return Path{};
    }
}

Path Path::homeDirectory() {
    try {
        return Path(fs::path(getenv("HOME") ?: "~").string());
    } catch (...) {
        return Path("~");
    }
}

bool operator==(const Path& lhs, const Path& rhs) noexcept {
    return lhs.native() == rhs.native();
}

bool operator!=(const Path& lhs, const Path& rhs) noexcept {
    return !(lhs == rhs);
}

} // namespace CodeWizard::Platform
