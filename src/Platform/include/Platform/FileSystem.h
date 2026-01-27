// include/Platform/FileSystem.h
#pragma once

#include "Path.h"
#include <Core/Result.h>
#include <Core/Types.h>
#include <vector>
#include <chrono>
using namespace CodeWizard::Core;

namespace CodeWizard::Platform {
struct FileInfo {
    Path path;
    bool isDirectory = false;
    bool isFile = false;
    std::uint64_t size = 0;
    std::chrono::system_clock::time_point lastModified;
};

enum class ReadOption {
    Text,
    Binary
};

enum class WriteOption {
    Text,
    Binary,
    Append
};

[[nodiscard]] Result<std::string> readFile(const Path& path, ReadOption option = ReadOption::Text);
[[nodiscard]] Result<void> writeFile(const Path& path, std::string_view content, WriteOption option = WriteOption::Text);
[[nodiscard]] Result<std::vector<FileInfo>> listDirectory(const Path& dir);
[[nodiscard]] Result<void> createDirectory(const Path& dir, bool recursive = false);
[[nodiscard]] Result<void> remove(const Path& path, bool recursive = false);
[[nodiscard]] Result<FileInfo> stat(const Path& path);
} // namespace CodeWizard::Platform
