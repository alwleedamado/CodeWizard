// include/Platform/Path.h
#pragma once

#include <Core/Types.h>
#include <string>
#include <string_view>

namespace CodeWizard::Platform {

class Path {
public:
    explicit Path(std::string_view path);
    Path() = default;
    [[nodiscard]] bool isAbsolute() const noexcept;
    [[nodiscard]] bool isRelative() const noexcept;
    [[nodiscard]] bool exists() const noexcept;
    [[nodiscard]] bool isDirectory() const noexcept;
    [[nodiscard]] bool isFile() const noexcept;

    [[nodiscard]] Path parent() const;
    [[nodiscard]] std::string filename() const;
    [[nodiscard]] std::string extension() const;
    [[nodiscard]] Path absolute() const;

    [[nodiscard]] Path operator/(const Path& other) const;
    [[nodiscard]] Path operator/(std::string_view other) const;

    [[nodiscard]] const std::string& native() const noexcept { return m_path; }

    [[nodiscard]] static Path currentWorkingDirectory();
    [[nodiscard]] static Path homeDirectory();

private:
    std::string m_path;
    void normalize();
};

bool operator==(const Path& lhs, const Path& rhs) noexcept;
bool operator!=(const Path& lhs, const Path& rhs) noexcept;

} // namespace CodeWizard::Platform
